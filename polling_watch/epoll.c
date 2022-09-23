#include "../../include/h_error.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>     // for fprintf()
#include <stdlib.h>    // for exit()
#include <string.h>    // for strlen()
#include <sys/epoll.h> // for epoll_create1()
#include <sys/types.h> // for pid_t
#include <unistd.h>    // for close()

#define MAX_EVENTS 10
#define READ_SIZE 10

// write in terminal for example
// https://suchprogramming.com/epoll-in-3-easy-steps/
int main() {
    int running = 1, event_count, i;
    int epoll_fd = epoll_create1(0);

    if (epoll_fd == -1) {
        fprintf(stderr, "Failed to create epoll file descriptor\n");
        return 1;
    }

    struct epoll_event event, events[MAX_EVENTS];
    event.events = EPOLLIN;
    event.data.fd = 0;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, 0, &event)) {
        fprintf(stderr, "Failed to add file descriptor to epoll\n");
        close(epoll_fd);
        return 1;
    }
    
    size_t bytes_read;
    char read_buffer[READ_SIZE];
    while (running) {
        printf("\nPolling for input...\n");
        event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, 10000);
        printf("%d ready events\n", event_count);
        for (i = 0; i < event_count; i++) {

            bytes_read = read(events[i].data.fd, read_buffer, READ_SIZE);
            printf("Reading file descriptor '%d' and %zd bytes read\n", events[i].data.fd, bytes_read);
            printf("Read: %s\n", read_buffer);

            if (!strncmp(read_buffer, "stop\n", 5))
                running = 0;
        }
    }

    if (close(epoll_fd)) {
        fprintf(stderr, "Failed to close epoll file descriptor\n");
        return 1;
    }

    return 0;
}