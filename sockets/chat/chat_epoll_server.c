/**
 * @file chat_epoll_server.c (part 1)
 * @author chiqui3d
 * @brief Simple server with socket y and epoll with edge triggered mode (nonblocking) to handle multiple clients
 * @version 0.1
 * @date 2022-07-01
 *
 * @references
 *
 * * https://web.archive.org/web/20160325155925/https://banu.com/blog/2/how-to-use-epoll-a-complete-example-in-c/
 * * https://eklitzke.org/blocking-io-nonblocking-io-and-epoll
 * * http://web.stanford.edu/~hhli/CS110Notes/CS110NotesCollection/Topic%204%20Networking%20(5).html
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "../../include/h_error.h"
#include <arpa/inet.h>  // for inet_ntoa()
#include <errno.h>      // for errno
#include <fcntl.h>      // for fcntl() nonblocking socket
#include <netdb.h>      // for getaddrinfo() and getservbyname
#include <signal.h>     // for sigaction
#include <stdio.h>      // for printf()
#include <stdlib.h>     // for exit()
#include <string.h>     // for strlen()
#include <sys/epoll.h>  // for epoll_create1()
#include <sys/socket.h> // for socket()
#include <unistd.h>     // for close()

#define BUFFER_SIZE 500
#define MAX_EPOLL_EVENTS 10
#define MAX_QUEUE_CONNECTIONS 20
#define MAX_CONNECTIONS 1000

static int makeSocketNonBlocking(int sfd);
static void acceptNewConnections(int epoll_fd, int socket_server_fd);
static void readFromClient(int epoll_fd, int client_fd);
static void writeToClient(int client_fd);
static struct epoll_event buildEvent(int events, int fd);

volatile sig_atomic_t sigintReceived = 0;
void sigint_handler(int s) { sigintReceived = 1; }

int main() {

    struct sigaction action = {};
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    action.sa_handler = sigint_handler;
    if (sigaction(SIGINT, &action, NULL) == -1) { // SIGINT: Ctrl + c signal
        fprintf(stderr, "sigaction");
    }

    int socket_server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_server_fd == -1) {
        die("socket");
    }

    int enable = 1;
    // avoid "Address already in use" error message (reuse port)
    if (setsockopt(socket_server_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof enable) == -1) {
        die("setsockopt");
    }

    // example_socket: 15557/tcp /etc/services
    struct servent *host = getservbyname("example_socket", "tcp");

    if (host == NULL) {
        die("getservbyname");
    }

    struct sockaddr_in server_address = {0};
    server_address.sin_family = AF_INET;
    server_address.sin_port = host->s_port;
    server_address.sin_addr.s_addr = INADDR_ANY;
    // INADDR_ANY: all available network interfaces on the machine,
    // and can be connected to by any clients on the server's LAN using the server's LAN IP or hostname
    // INADDR_LOOPBACK: Only clients on the same machine as the server can connect to it,
    if (bind(socket_server_fd, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        die("bind");
    }

    makeSocketNonBlocking(socket_server_fd);

    if (listen(socket_server_fd, MAX_QUEUE_CONNECTIONS) == -1) {
        printf("listen");
    }

    int epoll_fd = epoll_create1(0);
    if (epoll_fd < 0) {
        die("epoll_create failed");
    }
    // (EPOLLIN | EPOLLET Edge Triggered (ET) for non-blocking sockets
    struct epoll_event ev = buildEvent(EPOLLIN | EPOLLOUT | EPOLLET, socket_server_fd);
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_server_fd, &ev) < 0) {
        die("EPOLL_CTL_ADD failed");
    }

    struct epoll_event events[MAX_EPOLL_EVENTS];

    while (!sigintReceived) {
        char buffer[BUFFER_SIZE];
        int i, num_ready;
        num_ready = epoll_wait(epoll_fd, events, MAX_EPOLL_EVENTS, -1 /*timeout*/);
        for (i = 0; i < num_ready; i++) {
            if (events[i].data.fd == socket_server_fd) {
                printf("\n+++++++ Accepting new connection ++++++++\n\n");
                acceptNewConnections(epoll_fd, socket_server_fd);
            } else if (events[i].events & EPOLLIN) {
                printf("\n+++++++ Reading from client fd %i ++++++++\n\n", events[i].data.fd);
                readFromClient(epoll_fd, events[i].data.fd);
            } else if (events[i].events & EPOLLOUT) {
                writeToClient(events[i].data.fd);
            } else if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP)) {
                perror("EPOLLERR|EPOLLHUP");
                close(events[i].data.fd);
            }
        }
    }

    printf("\n+++++++ Closing server ++++++++\n\n");
    close(epoll_fd);
    close(socket_server_fd);

    return EXIT_SUCCESS;
}

static int makeSocketNonBlocking(int sfd) {
    int flags, s;

    flags = fcntl(sfd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl");
        return -1;
    }

    flags |= O_NONBLOCK;
    s = fcntl(sfd, F_SETFL, flags);
    if (s == -1) {
        perror("fcntl");
        return -1;
    }

    return 0;
}
static struct epoll_event buildEvent(int events, int fd) {
    struct epoll_event event = {0};
    event.events = events;
    event.data.fd = fd;
    return event;
}
static void writeToClient(int client_fd) {
    char response[] = "Ok\n";
    write(client_fd, response, strlen(response) + 1);
    printf("\nWritten to client fd%i: %s\n\n", client_fd, response);
}

static void acceptNewConnections(int epoll_fd, int socket_server_fd) {
    while (1) {
        struct sockaddr_in client_address;
        socklen_t client_address_len = sizeof(client_address);
        int client_fd = accept(socket_server_fd, (struct sockaddr *)&client_address, (socklen_t *)&client_address_len);
        // alternative to makeSocketNonBlocking
        // int client_fd = accept4(socket_server_fd, (struct sockaddr *)&client_address, (socklen_t *)&client_address_len, SOCK_NONBLOCK);

        if (client_fd < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // This can happen due to the nonblocking socket mode; in this
                // case don't do anything, but print a notice (since these events
                // are extremely rare and interesting to observe...)
                break;
            } else {
                perror("accept client connection");
                break;
            }
        }
        if (client_fd >= MAX_CONNECTIONS) {
            close(client_fd);
            die("socket fd (%d) >= MAX_CONNECTIONS (%d)", client_fd, MAX_CONNECTIONS);
        }

        printf("Connect with the client %d %s:%d\n", client_fd, inet_ntoa(client_address.sin_addr), htons(client_address.sin_port));
        makeSocketNonBlocking(client_fd);
        struct epoll_event event = buildEvent(EPOLLIN | EPOLLET, client_fd);
        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event) < 0) {
            die("EPOLL_CTL_ADD failed");
        }
    }
}
static void readFromClient(int epoll_fd, int client_fd) {
    /* We have data on the fd waiting to be read. Read and
        display it. We must read whatever data is available
        completely, as we are running in edge-triggered mode
        and won't get a notification again for the same
        data. */
    int done = 0;
    while (1) {
        char buffer[BUFFER_SIZE];
        int bytes_read = read(client_fd, buffer, BUFFER_SIZE);
        if (bytes_read == -1) {
            /* If errno == EAGAIN, that means we have read all
               data. So go back to the main loop. */
            if (errno != EAGAIN && errno != EWOULDBLOCK) {
                perror("read");
                done = 1;
            }
            break;
        } else if (bytes_read == 0) {
            /* End of file. The remote has closed the connection. */
            done = 1;
            break;
        }

        printf("Client: %s\n", buffer);
        if (strncmp(buffer, "exit", 4) == 0) {
            done = 1;
            break;
        }
    }
    if (done) {
        if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, NULL) < 0) {
            die("EPOLL_CTL_DEL failed");
        }
        close(client_fd);
        printf("\n+++++++ Close connection for client fd %i ++++++++\n\n", client_fd);
    } else {
        struct epoll_event event = buildEvent(EPOLLOUT | EPOLLIN | EPOLLET, client_fd);
        if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, client_fd, &event) < 0) { // MOD == modify existing event for write (EPOLLOUT)
            die("EPOLL_CTL_MOD failed");
        }
    }
}