#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t child_pid;
    /* Create a child process. */
    printf("the main program process ID is %d\n", (int)getpid());
    child_pid = fork();
    if (child_pid > 0) {
        /* This is the parent process. Sleep for a minute. */
        printf("The parent process ID is %d\n", (int)getpid());
        sleep(60);
    } else {
        /* This is the child process. Exit immediately. */
        printf("The child process ID is %d\n", (int)getpid());
        exit(0);
    }
    return 0;
}

// ps -e -o pid,ppid,stat,cmd for see zombie process | 31825 31824 Z+   [4zombie] <defunct>