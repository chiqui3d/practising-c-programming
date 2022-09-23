/** ./process-pipe-fork/execve/execve_loop.c **/

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "become_daemon.h"

/* SIGCHLD handler to reap dead child processes */
static void grimReaper(int signum, siginfo_t *sinfo, void *unused) {
    /* Save 'errno' in case changed here */
    int savedErrno = errno;
    while (waitpid(-1, NULL, WNOHANG) > 0) {
        printf("child %d exited\n", (int)sinfo->si_pid);
        continue;
    }

    errno = savedErrno;
}

int main(int argc, char *argv[]) {

    setbuf(stdout, NULL); // make stdout unbuffered

    char *newArgv[] = {"myecho_bg", "HELLO", "WORLD", NULL};
    char *newEnviron[] = {"MY_NAME=jcarlosweb", NULL};

    struct sigaction sa;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_SIGINFO;
    sa.sa_sigaction = grimReaper;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        execl("./process-pipe-fork/execve/myecho_bg", "./process-pipe-fork/execve/myecho_bg","HELLO5555", "WORLD", NULL);
        /* The execve function returns only if an error occurs. */
        exit(EXIT_FAILURE);
    }

    usleep(1);
    // continue execution of parent process without waiting for child process to finish
    waitpid(-1, NULL, WNOHANG);

    printf("Parent process: %d\n", getppid());
    printf("Done\n");

    exit(EXIT_SUCCESS);
}