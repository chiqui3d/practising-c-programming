#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    int inotify_fd = inotify_init();
    if (inotify_fd == -1) {
        perror("inotify_init(): ");
    }

    int watch_descriptor = inotify_add_watch(inotify_fd, "/proc", IN_ALL_EVENTS);
    //int watch_descriptor = inotify_add_watch(inotify_fd, "/proc", IN_ALL_EVENTS);
    if (watch_descriptor == -1) {
        perror("inotify_add_watch(): ");
    }

    struct inotify_event *event;
    char buf[1024];
    ssize_t result;

    // Watch forever, until signal (CTRL-C, etc.)
    while (1) {
        printf("Hello: \n");
        result = read(inotify_fd, buf, sizeof(buf));
       if (result == -1 && errno != EAGAIN) {
            perror("read(): ");
            exit(EXIT_FAILURE);
       }


        // Print all events read from inotify_fd:
        int index = 0;
        while (index < result) {
            event = (void *)(&buf[index]);
            printf("index %d: %s\n", (int)index, event->name);
            index += sizeof(struct inotify_event) + event->len;
        }
        printf("\n");
    }
}