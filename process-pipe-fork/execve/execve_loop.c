/** ./process-pipe-fork/execve/execve_loop.c **/

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

volatile sig_atomic_t runChild = 0;


static void handler(int signum, siginfo_t *sinfo, void *unused){

    if (signum == SIGUSR1) {
        printf("SIGUSR1 received\n");
        runChild = 1;
    }
}

int main(int argc, char *argv[]) {

    char *newArgv[] = {"myecho", "hello", "world", NULL};
    char *newEnviron[] = { "MY_NAME=jcarlosweb", NULL };

    struct sigaction sa;
    sa.sa_flags =0;
    sigemptyset(&sa.sa_mask);

    sa.sa_sigaction = handler;
    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        while (!runChild) {
            usleep(200);
        }
        runChild = 0;
        execve("./process-pipe-fork/execve/myecho", newArgv, newEnviron);
        /* The execve function returns only if an error occurs. */
        perror("An error occurred in execve");
        abort();
    }
    
    usleep(1);
    // continue execution of parent process without waiting for child process to finish
    waitpid(-1, NULL, WNOHANG);

    printf("Run myecho:\n");
    kill(pid,SIGUSR1);

    sleep(3);

    printf("Run myecho:\n");
    if (kill(pid,SIGUSR1) == -1) {
        perror("kill");
    }

    printf("Done\n");

    exit(EXIT_SUCCESS);
}