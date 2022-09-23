#include "../include/h_error.h"
#include "../include/h_prompt.h"
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h> // for getaddrinfo() and getservbyname
#include <netinet/in.h>
#include <stdio.h>      // for fprintf()
#include <stdlib.h>     // for exit()
#include <string.h>     // for strlen()
#include <sys/socket.h> // for socket()
#include <sys/types.h>  // for pid_t
#include <unistd.h>     // for close()

#define BUF_SIZE 4094

#define SEND_SIZE 1024

#define DOMAIN "www.example.com"

#define HEADER "GET / HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n"

int main() {

    struct addrinfo hints, *result, *rp;

    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;
    hints.ai_family = AF_UNSPEC; /* Allow IPv4 or IPv6 */

    int rc = getaddrinfo(DOMAIN, "www", &hints, &result);

    if (rc != 0) {
        printf("getaddrinfo: %s\n", gai_strerror(rc));
        exit(EXIT_FAILURE);
    }

    int socket_fd;
    for (rp = result; rp != NULL; rp = rp->ai_next) {

        socket_fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

        if (socket_fd == -1) {
            continue;
        }

        if (connect(socket_fd, rp->ai_addr, rp->ai_addrlen) == 0) {
            break; // Success
        }
        close(socket_fd);
    }

    if (rp == NULL) {
        die("connect failed");
    }

    freeaddrinfo(result);
    printf("Yeah! Connected with %s\n", DOMAIN);

    char send_data[SEND_SIZE];
    snprintf(send_data, sizeof(send_data), HEADER, DOMAIN);

    write(socket_fd, send_data, strlen(send_data));

    char buf[BUF_SIZE];
    ssize_t n;
    while ((n = read(socket_fd, buf, BUF_SIZE))) {
        if (n < 0) {
            die("read");
        }
        printf("Read: %.*s", (int)n, buf);
    }

    close(socket_fd);

    return EXIT_SUCCESS;
}