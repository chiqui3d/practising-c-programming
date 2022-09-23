#include "../../include/h_error.h"
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>      // for getaddrinfo() and getservbyname
#include <stdio.h>      // for fprintf()
#include <stdlib.h>     // for exit()
#include <string.h>     // for strlen()
#include <sys/socket.h> // for socket()
#include <sys/types.h>  // for pid_t
#include <unistd.h>     // for close()

#define BUF_SIZE 500

int main() {

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

    if (bind(socket_server_fd, (struct sockaddr *)&address_in, sizeof(address_in)) == -1) {
        die("Error bind");
    }
    // number of connections
    if (listen(socket_server_fd, 2) == -1) {
        printf("listen");
    }

    struct sockaddr client = {0};
    socklen_t client_len;

    int client_fd = accept(socket_server_fd, &client, &client_len);
    if (client_fd == -1) {
        die("accept");
    }

    char buffer[BUF_SIZE];

    if(read(client_fd, buffer, BUF_SIZE) > 0) {
        printf("Server Read from client: %s\n", buffer);
    }else{
        die("read from client");
    }

    strcpy(buffer, "Bye World12");
    // write to client
    write(client_fd, buffer, strlen(buffer) + 1);
    printf("Server Write to client: %s\n", buffer);

    close(client_fd);
    close(socket_server_fd);

    return EXIT_SUCCESS;
}