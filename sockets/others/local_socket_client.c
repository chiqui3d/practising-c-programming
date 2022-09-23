#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
/* Write TEXT to the socket given by file descriptor SOCKET_FD. */
void write_text(int socket_fd, const char *text) {
    /* Write the number of bytes in the string, including
    NUL-termination. */
    int length = strlen(text) + 1;
    write(socket_fd, &length, sizeof(length));
    /* Write the string. */
    write(socket_fd, text, length);
}
int main(int argc, char *const argv[]) {
    char socket_name[] = "/tmp/123.socket";

    if (argc != 2) {
        fprintf(stderr, "Usage: %s TEXT\n", argv[0]);
        return 1;
    }

    char *message = argv[1];
    int socket_fd;
    struct sockaddr_un name = {0};
    /* Create the socket. */
    socket_fd = socket(PF_LOCAL, SOCK_STREAM, 0);
    /* Store the server’s name in the socket address. */

    /*
     * For portability clear the whole structure, since some
     * implementations have additional (nonstandard) fields in
     * the structure.
     */

    memset(&name, 0, sizeof(name));
    name.sun_family = AF_LOCAL;
    strcpy(name.sun_path, socket_name);
    /* Connect the socket. */
    connect(socket_fd, (const struct sockaddr *) &name, sizeof(name));
    /* Write the text on the command line to the socket. */
    write_text(socket_fd, message);
    close(socket_fd);
    return 0;
}