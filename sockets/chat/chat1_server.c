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
        printf("listen");
    }

    /** Epoll begin for timeout **/
    int epoll_fd = epoll_create(1);
    if (epoll_fd < 0) {
        die("epoll_create failed");
    }

    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = socket_server_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_server_fd, &ev) < 0) {
        die("epoll_ctl failed");
    }

    printf("\n+++++++ Waiting for new connection ++++++++\n\n");
    struct epoll_event event;
    int res;
    if ((res = epoll_wait(epoll_fd, &event, MAX_EVENTS, 10000)) <= 0) {
        if (res < 0) {
            die("epoll_wait failed");
        } else {
            printf("\n+++++++ Timeout ++++++++\n\n");
        }
        close(epoll_fd);
        close(socket_server_fd);
        exit(EXIT_SUCCESS);
    }

    close(epoll_fd);
    /** Epoll end **/

    struct sockaddr client_address = {0};
    socklen_t client_address_len = sizeof(client_address);
    int client_fd;
    while (1) {
        client_fd = accept(socket_server_fd, (struct sockaddr *)&client_address, (socklen_t *)&client_address_len);
        if (client_fd == -1) {
            die("accept");
        }
        char buffer[BUF_SIZE];
        while (read(client_fd, buffer, BUF_SIZE) > 0) {
            printf("Client: %s\n", buffer);
            if (strncmp(buffer, "stop", 4) == 0 || strncmp(buffer, "close", 5) == 0) {
                printf("Close server\n");
                close(client_fd);
                close(socket_server_fd);
                return EXIT_SUCCESS;
            }

            strcpy(buffer, "OK");
            write(client_fd, buffer, strlen(buffer) + 1);
        }
    }

    close(client_fd);
    close(socket_server_fd);

    return EXIT_SUCCESS;
}