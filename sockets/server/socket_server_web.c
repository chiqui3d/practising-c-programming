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

#include "llhttp.h"

#define BUF_SIZE 1024
#define MAX_EVENTS 10

// https://blog.cloudflare.com/the-sad-state-of-linux-socket-balancing/
// https://users.cs.duke.edu/~chase/cps196/slides/sockets.pdf
// https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa
// https://beej.us/guide/bgnet/
// https://web.dev/howbrowserswork/

int handle_on_message_complete() {
    printf("on_message_complete\n");
    return 0;
}

llhttp_t parseRequest(char *request, int bytes_read) {

    llhttp_t parser;
    llhttp_settings_t settings;

    /* Initialize user callbacks and settings */
    llhttp_settings_init(&settings);

    /* Set user callback */
    // settings.on_message_complete = handle_on_message_complete;

    /* Initialize the parser in HTTP_BOTH mode, meaning that it will select between
     * HTTP_REQUEST and HTTP_RESPONSE parsing automatically while reading the first
     * input.
     */
    llhttp_init(&parser, HTTP_BOTH, &settings);

    enum llhttp_errno err = llhttp_execute(&parser, request, bytes_read);
    if (err != HPE_OK) {
        fprintf(stderr, "Parse error: %s %s\n", llhttp_errno_name(err), parser.reason);
    }

    return parser;
}

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
    
    struct sockaddr_in address_in = {0};
    address_in.sin_family = AF_INET;
    address_in.sin_port = host->s_port;
    address_in.sin_addr.s_addr = INADDR_ANY; // bind to any address on this machine
    socklen_t address_len = sizeof(address_in);

    if (bind(socket_server_fd, (struct sockaddr *)&address_in, address_len) == -1) {
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

    // close(epoll_fd);
    /** Epoll end **/

    long client_fd;
    while (1) {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((client_fd = accept(socket_server_fd, (struct sockaddr *)&address_in, (socklen_t *)&address_len)) < 0) {
            close(socket_server_fd);
            die("In accept");
        }

        char buffer[BUF_SIZE];
        int bytes_read;
        if ((bytes_read = read(client_fd, buffer, BUF_SIZE)) < 0) {
            die("In read");
        }

        llhttp_t parser = parseRequest(buffer, bytes_read);

        const char *method = llhttp_method_name(parser.method);
        if (parser.error != HPE_OK) {
            fprintf(stderr, "Parse error: %s %s\n", llhttp_errno_name(parser.error), parser.reason);
        }

        if (strncmp(method, "GET", 3) == 0 && parser.reason == HPE_OK) {
            char *okBrowser = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
            strcpy(buffer, okBrowser);
            printf("Method: %s\n", method);
            printf("Buffer:\n %s\n", buffer);
        } else {
            printf("Client: %s\n", buffer);
            strcpy(buffer, "OK");
        }

        // write to client
        write(client_fd, buffer, strlen(buffer) + 1);
        close(client_fd); // cerramos el socket cliente, si es para chat se puede quitar y poner al final del programa
    }

    return EXIT_SUCCESS;
}