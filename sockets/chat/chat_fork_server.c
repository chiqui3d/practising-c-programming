#include "../../include/h_error.h"
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>      // for getaddrinfo() and getservbyname
#include <stdio.h>      // for fprintf()
#include <stdlib.h>     // for exit()
#include <string.h>     // for strlen()
#include <sys/epoll.h>  // for epoll_create1()
#include <sys/socket.h> // for socket()
#include <sys/types.h>  // for pid_t
#include <unistd.h>     // for close()
#include <stdbool.h>    // for bool

#define BUF_SIZE 500
#define MAX_EVENTS 10

// https://blog.cloudflare.com/the-sad-state-of-linux-socket-balancing/
// https://users.cs.duke.edu/~chase/cps196/slides/sockets.pdf
// https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa

int main() {

    int running = 1;
    int socket_server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_server_fd == -1) {
        die("socket");
    }

    // example_socket: 15557/tcp

    struct servent *host = getservbyname("example_socket", "tcp"); // /etc/services

    if (host == NULL) {
        die("getservbyname");
    }

    struct sockaddr_in server_address = {0};
    server_address.sin_family = AF_INET;
    server_address.sin_port = host->s_port;
    server_address.sin_addr.s_addr = INADDR_ANY; // bind to any address on this machine

    if (bind(socket_server_fd, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        die("bind");
    }
    // 3 number of connections
    if (listen(socket_server_fd, 3) == -1) {
        die("listen");
    }

    printf("\n+++++++ Waiting for new connection ++++++++\n\n");

    int client_fd;
    pid_t pid;
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    bool running = true;
    while (running) {
        client_fd = accept(socket_server_fd, (struct sockaddr *)&client_address, &client_address_len);
        if (client_fd < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
            // This can happen due to the nonblocking socket mode; in this
            // case don't do anything, but print a notice (since these events
            // are extremely rare and interesting to observe...)
            printf("accept returned EAGAIN or EWOULDBLOCK\n");
          } else {
            exit(EXIT_FAILURE);
          }
        }

        if ((pid = fork()) == -1) {
            die("fork");
        }
        
        if (pid == 0) {
            printf(
                "pid=%d, ppid=%d, clientFd=%d ,socketFd=%d, clientPort=%d\n", 
                getpid(), getppid(), client_fd, socket_server_fd, htons(client_address.sin_port)
            );
            close(socket_server_fd);
            char buffer[BUF_SIZE];
            while (read(client_fd, buffer, BUF_SIZE) > 0) {
                printf("Client: %s\n", buffer);
                if (strncmp(buffer, "exit", 4) == 0) {
                    printf("Close fd: %i\n", getpid());
                    close(client_fd);
                    break;
                }
                strcpy(buffer, "OK");
                write(client_fd, buffer, strlen(buffer) + 1);
            }
        } else {
            // parent process
            close(client_fd);
        }
    }

    close(client_fd);
    close(socket_server_fd);

    return EXIT_SUCCESS;
}