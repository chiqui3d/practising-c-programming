/** ./process-pipe-fork/execve/execve_loop.c **/

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

volatile sig_atomic_t runChild = 1;

static void activeChildHandler(int signum, siginfo_t *sinfo, void *unused) {
    runChild = 0;
}
static void handleChildTERM(int signum, siginfo_t *sinfo, void *unused)
{

    printf("Child %d exited\n", (int)sinfo->si_pid);
    //waitpid(sinfo->si_pid, NULL, 0);
}

int main(int argc, char *argv[]) {

    setbuf(stdout, NULL); // make stdout unbuffered

    char *newArgv[] = {"myecho", "hello", "world", NULL};
    char *newEnviron[] = {"MY_NAME=jcarlosweb", NULL};

    struct sigaction sa;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    sa.sa_sigaction = activeChildHandler;
    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction SIGUSR1");
        exit(EXIT_FAILURE);
    }
    sa.sa_flags = SA_SIGINFO | SA_NOCLDSTOP;
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = handleChildTERM;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction SIGCHLD");
        exit(EXIT_FAILURE);
    }

    char buffer[1024];
    int fd[2];
    pipe(fd); /* to block on */

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        close(fd[1]);
        printf("Child process: %d, fd[0]: %d, fd[1]: %d\n", getpid(), fd[0], fd[1]);
        while (runChild) {
            ssize_t nbytes = read(fd[0], buffer, sizeof(buffer));
            if (nbytes > 0) {
                printf("Buffer: %s\n", buffer);
                //execve("./process-pipe-fork/execve/myecho", newArgv, newEnviron);
            }
        }
        close(fd[0]);
        exit(EXIT_SUCCESS);
    }

    // continue execution of parent process without waiting for child process to finish
    waitpid(-1, NULL, WNOHANG);
    close(fd[0]);

    if (write(fd[1], "Hello Carlos\n", 14) == -1) {
        perror("write");
    }

    sleep(1);
    if (write(fd[1], "Hello World\n", 13) == -1) {
        perror("write");
    }

    sleep(1);
    if (write(fd[1], "Bye Bye\n", 9) == -1) {
        perror("write");
    }

    sleep(1);
    printf("Parent process: %d, fd[0]: %d, fd[1]: %d\n", getppid(), fd[0], fd[1]);

    kill(pid, SIGUSR1);
    close(fd[1]);

    sleep(1);
    printf("Done\n");

    exit(EXIT_SUCCESS);
}