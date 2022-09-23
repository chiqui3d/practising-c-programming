#include "../../include/h_error.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
    int fd[2];
    pid_t pid;
    char string[] = "Hello, world!\n";
    char buffer[100];

    if (pipe(fd) == 1) {
        die("pipe");
    }

    if ((pid = fork()) == -1) {
        die("fork");
    }

    if (pid == 0) {
        printf("Child Read\n");
        close(fd[1]);

        while ((read(fd[0], buffer, sizeof(buffer))) > 0) {
            printf("Received string: %s", buffer);
        }

        close(fd[0]);

        return EXIT_SUCCESS;
    } else {
        printf("Parent Write\n");
        close(fd[0]);
        write(fd[1], string, strlen(string) + 1);
        close(fd[1]);
        return EXIT_SUCCESS;
    }

    return (0);
}