#include <errno.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
// #include <sys/signalfd.h>
#include <signal.h>
#include "../../include/h_error.h"

// https://man7.org/linux/man-pages/man2/poll.2.html
volatile sig_atomic_t sigintReceived = 0;

void sigint_handler(int s) { sigintReceived = 1; }

void printPollEvent(int fd, int revents) {
    printf("fd=%d --> ", fd);

    if (revents & POLLIN)
        printf("POLLIN ");
    if (revents & POLLPRI)
        printf("POLLPRI ");
    if (revents & POLLOUT)
        printf("POLLOUT ");
    if (revents & POLLERR)
        printf("POLLERR ");
    if (revents & POLLHUP)
        printf("POLLHUP ");
    if (revents & POLLNVAL)
        printf("POLLNVAL");
    printf("\n");
}
int main(int argc, char *argv[]) {

    char file[] = "_data/poll_input.txt";
   
    int fdLog = open(file, O_RDONLY);

    if (fdLog == -1 && errno != EINTR) {
        fprintf(stderr, "open '%s': %s\n", file, strerror(errno));
        exit(EXIT_FAILURE);
    }

    printf("pid=%d, ppid=%d, fd=%d\n", getpid(), getppid(), fdLog);
    printf("Check /proc/%d/fd/%d\n", getpid(), fdLog);

    int num_open_fds = 1;
    struct pollfd pfds = {
        .fd = fdLog,
        .events = POLLIN,
    };

    /*  pfds[0].fd = sfd;
    pfds[0].events = POLLIN | POLLERR | POLLHUP;
    pfds[1].fd = fdLog;
    pfds[1].events = POLLIN; */

    struct sigaction action = {};
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    action.sa_handler = sigint_handler;
    if (sigaction(SIGINT, &action, NULL) == -1) {
        fprintf(stderr, "sigaction");
    }

    while (!sigintReceived) {

        int ready = poll(&pfds, 1, -1);

    
        if (ready == -1) {
            die("poll");
        }

        char buffer[1024];
        if (pfds.revents != 0  && !sigintReceived) {
            if ((pfds.revents & POLLIN) && !sigintReceived) {
                ssize_t s;
                while ((s = read(pfds.fd, buffer, sizeof(buffer))) > 0) {
                    printPollEvent(pfds.fd, pfds.revents);
                    write(STDOUT_FILENO, buffer, s);
                    //  printf("read %zd bytes: %.*s\n", s, (int)s, buffer);
                }
                if (s == -1 && errno != EAGAIN && errno != EINTR) {
                    die("read");
                }

            } else { /* POLLERR | POLLHUP */
                printf("closing fd %d\n", pfds.fd);
                close(pfds.fd);
            }
        }
    }

    
    close(fdLog);
    printf("\nAll file descriptors closed\n");
    return EXIT_SUCCESS;
}