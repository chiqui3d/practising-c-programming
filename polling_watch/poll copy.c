#include "../../include/h_error.h"
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/signalfd.h>
#include <sys/types.h>
#include <unistd.h>

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

    int num_open_fds = 2;

    struct pollfd pfds[2];
    sigset_t mask;
    struct signalfd_siginfo siginfo;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGQUIT);

    /* Block signals so that they aren't handled
        according to their default dispositions. */

    if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1) {
        die("sigprocmask");
    }

    int sfd = signalfd(-1, &mask, 0);
    if (sfd == -1) {
        die("signalfd");
    }

    pfds[0].fd = sfd;
    pfds[0].events = POLLIN | POLLERR | POLLHUP;
    pfds[1].fd = fdLog;
    pfds[1].events = POLLIN;

    while (1) {
        
        pfds[0].revents = 0;
        pfds[1].revents = 0;
        int ready = poll(pfds, num_open_fds, -1);
        if (ready == -1) {
            die("poll");
        }
        char buffer[1024];

        if (pfds[0].revents & POLLIN) {
            while (read(pfds[0].fd, &siginfo, sizeof(siginfo)) != sizeof(siginfo)) {
                die("read siginfo");
            }
            break;
        }

        if ((pfds[1].revents & POLLIN)) {
            ssize_t s;
            while ((s = read(pfds[1].fd, buffer, sizeof(buffer))) > 0) {
                printPollEvent(pfds[1].fd, pfds[1].revents);
                write(STDOUT_FILENO, buffer, s);
                //  printf("read %zd bytes: %.*s\n", s, (int)s, buffer);
            }
            if (s == -1 && errno != EAGAIN && errno != EINTR) {
                die("read");
            }

        } else { /* POLLERR | POLLHUP */
            printf("closing fd %d\n", pfds[1].fd);
            close(pfds[1].fd);
        }
    }

    close(sfd);
    close(fdLog);
    printf("All file descriptors closed\n");
    exit(EXIT_SUCCESS);
}