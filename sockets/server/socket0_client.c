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
#include <netinet/in.h>
#include <arpa/inet.h>


#define BUF_SIZE 500


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

    struct sockaddr_in address_in = {0};
    address_in.sin_family = AF_INET;
    address_in.sin_port = port->s_port;
    address_in.sin_addr.s_addr = ((struct in_addr *)(host->h_addr))->s_addr;

    if (connect(socket_fd, (struct sockaddr *)&address_in, sizeof(address_in)) == -1) {
        die("connect");
    }

    printf("Connect with %s:%d\n",inet_ntoa(address_in.sin_addr),htons(address_in.sin_port));


    char buffer[BUF_SIZE];
    strcpy(buffer, "Hello World");

    // write to socket
    write(socket_fd, buffer, strlen(buffer) + 1);
    printf("Client Write to server: %s\n", buffer);

    // read info send by server

    while(read(socket_fd, buffer, BUF_SIZE) > 0) {
        printf("Client read from server: %s\n", buffer);
    }

    close(socket_fd);

    return EXIT_SUCCESS;
}