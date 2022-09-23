#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

sigset_t signalWait;
int signalWaitInfo;

static void handleChildTERM(int signum, siginfo_t *sinfo, void *unused) {
    printf("Child %d exited\n", (int)sinfo->si_pid);
    signal(SIGCHLD, SIG_DFL);
    sigprocmask(SIG_UNBLOCK, &signalWait, NULL);

}

int main(int argc, char *argv[]) {

    struct sigaction sa;

    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = handleChildTERM;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction SIGCHLD");
        exit(EXIT_FAILURE);
    }


    sigemptyset(&signalWait);
    sigaddset(&signalWait, SIGUSR2);
    sigprocmask(SIG_BLOCK, &signalWait, NULL);
   /*  A  signal  may  be  blocked,  which means that it will not be delivered
    until it is later unblocked or sigwait.  Between the time when it is generated and
    when it is delivered a signal is said to be pending. */

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {
        printf("Hello from child before running sigwait\n");
        sigwait(&signalWait, &signalWaitInfo);
        printf("Child process started: %d\n", getpid());
        _exit(EXIT_SUCCESS);
    }

    printf("Parent process: %d...\n", getpid());
    sleep(3);
    // send signal to child process for it to continue
    kill(pid, SIGUSR2);
    // When SIGCHLD is caught, sleep(3) is interrupted and the child process is terminated
    // https://stackoverflow.com/questions/14266485/understanding-sigchld-when-the-child-process-terminates
    unsigned short int sleepTime = 3;
    while((sleepTime = sleep(sleepTime)) > 0);
    printf("Done with main program\n");

    exit(EXIT_SUCCESS);
}