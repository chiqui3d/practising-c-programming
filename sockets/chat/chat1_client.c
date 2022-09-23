#include "../../include/h_error.h"
#include "../../include/h_prompt.h"
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h> // for getaddrinfo() and getservbyname
#include <netinet/in.h>
#include <stdio.h>      // for fprintf()
#include <stdlib.h>     // for exit()
#include <string.h>     // for strlen()
#include <sys/epoll.h>  // for epoll_create1()
#include <sys/socket.h> // for socket()
#include <sys/types.h>  // for pid_t
#include <unistd.h>     // for close()

#define BUF_SIZE 500
#define MAX_EPOLL_EVENTS 10

int main() {

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_fd == -1) {
        die("socket");
    }

    // localhost

    struct hostent *host = gethostbyname("localhost"); // /etc/hosts

    if (host == NULL) {
        die("gethostbyname");
    }

    // example_socket: 15557/tcp

    struct servent *port = getservbyname("example_socket", "tcp"); // /etc/services

    if (port == NULL) {
        die("getservbyname");
    }

    /**
     * @brief Add socket to epoll
     *
     * Epoll:
     * refers to a constant query operation, usually to a hardware device,
     * to create a synchronous activity without the use of interrupts
     * Here we use epoll to see if the connection has been closed or if we have a new message from the server
     *
     */
    int epoll_fd = epoll_create(1);
    struct epoll_event event;
    struct epoll_event events[MAX_EPOLL_EVENTS];
    event.events = EPOLLIN; // Can append "|EPOLLOUT" for write events as well
    event.data.fd = socket_fd;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &event)) {
        close(epoll_fd);
        close(socket_fd);
        die("Failed to add socket descriptor to epoll");
    }

    struct sockaddr_in address_in = {0};
    address_in.sin_family = AF_INET;
    address_in.sin_port = port->s_port;
    address_in.sin_addr.s_addr = ((struct in_addr *)(host->h_addr))->s_addr;

    if (connect(socket_fd, (struct sockaddr *)&address_in, sizeof(address_in)) == -1) {
        close(socket_fd);
        die("connect");
    }

    
    printf("Connect with %s:%d in the socket_fd %i\n", inet_ntoa(address_in.sin_addr), htons(address_in.sin_port), socket_fd);

    int i, num_ready;
    while (1) {
        char buffer[BUF_SIZE];
        char *chat;
        chat = getString("(you) Enter any word:");
        strcpy(buffer, chat);
        free(chat);
        write(socket_fd, buffer, strlen(buffer) + 1);
        /*---Wait for data---*/
        num_ready = epoll_wait(epoll_fd, events, MAX_EPOLL_EVENTS, 5000 /*timeout*/);
        for (i = 0; i < num_ready; i++) {
            if (events[i].events & EPOLLIN) {
                int bytes_read = read(events[i].data.fd, buffer, BUF_SIZE);
                if (bytes_read == 0) {
                    printf("Socket disconnected\n");
                    close(epoll_fd);
                    close(socket_fd);
                    return EXIT_SUCCESS;
                } else if (bytes_read == -1) {
                    close(epoll_fd);
                    close(socket_fd);
                    die("read");
                } else {
                    printf("Server: %s\n", buffer);
                }
            }
        }
    }

    close(epoll_fd);
    close(socket_fd);

    return EXIT_SUCCESS;
}