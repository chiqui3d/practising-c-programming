#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <unistd.h>
#include "../include/h_error.h"

/* Read characters from the pipe and echo them to stdout. */
// https://www.gnu.org/software/libc/manual/html_node/Creating-a-Pipe.html

void read_from_pipe(int file) {
    int c;
    FILE *stream = fdopen(file, "r");
    while ((c = fgetc(stream)) != EOF) {
        putchar(c);
    }
    fclose(stream);
}

/* Write some random text to the pipe. */

void write_to_pipe(int file) {
    FILE *stream = fdopen(file, "w");
    fprintf(stream, "hello, world!\n");
    fprintf(stream, "goodbye, world!\n");
    fclose(stream);
}

int main(void) {
    int mypipe[2];

    /* Create the pipe. */
    if (pipe(mypipe) == -1) {
        fprintf(stderr, "Pipe failed.\n");
        return EXIT_FAILURE;
    }

    
    /* Create the child process. */
    pid_t pid = fork();

   
    struct rlimit limit;
    if (getrlimit(RLIMIT_NPROC, &limit) == -1) {
        die("getrlimit failed.\n");
    }

    printf("OLD RLIMIT_NPROC: %lu\n", limit.rlim_cur);

    limit.rlim_cur = limit.rlim_max = 32000;

    if (setrlimit(RLIMIT_NPROC, &limit) == -1) {
        die("setrlimit failed");
    }

    if (getrlimit(RLIMIT_NPROC, &limit) == -1) {
        die("getrlimit failed.\n");
    }
    
    printf("NEW RLIMIT_NPROC: %lu\n", limit.rlim_cur);
    //system("bash -c 'ulimit -a'");
    printf("pid=%d, ppid=%d, childid=%d, fd0=%d ,fd1=%d\n", getpid(), getppid(), pid, mypipe[0], mypipe[1]);

    if (pid == (pid_t)0) {
        printf("read\n");
        /* This is the child process. Close other end first. */
        close(mypipe[1]);
        read_from_pipe(mypipe[0]);
        return EXIT_SUCCESS;
    } else if (pid < (pid_t)0) {
        /* The fork failed. */
        fprintf(stderr, "Fork failed.\n");
        return EXIT_FAILURE;
    } else {
        printf("write\n");
        /* This is the parent process. Close other end first. */
        close(mypipe[0]);
        write_to_pipe(mypipe[1]);
        return EXIT_SUCCESS;
    }
}
