#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static int *runChildProcess;

sigset_t signalWait;
int signalWaitInfo;

static void handleChildTerm(int signum, siginfo_t *sinfo, void *unused) {
    printf("Child %d exited\n", (int)sinfo->si_pid);
}

static void runChild(pid_t pid) {
    // send signal to child process for it to continue
    kill(pid, SIGUSR2);
    // When SIGCHLD is caught, sleep(3) is interrupted and the child process is terminated
    // https://stackoverflow.com/questions/14266485/understanding-sigchld-when-the-child-process-terminates
    unsigned short int sleepTime = 3;
    while ((sleepTime = sleep(sleepTime)) > 0)
        ;
}

static void termChild(pid_t pid) {
    *runChildProcess = 0; // set to 0 to exit the loop
    runChild(pid); // unblock sigwait
    sigprocmask(SIG_UNBLOCK, &signalWait, NULL); // unblock SIGCHLD signal
    signal(SIGCHLD, SIG_DFL);                    // restore default signal handler
    munmap(runChildProcess, sizeof *runChildProcess);
}

int main(int argc, char *argv[]) {

    runChildProcess = mmap(NULL, sizeof *runChildProcess, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    if (runChildProcess == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    *runChildProcess = 1; // set to 1 to run the loop

    struct sigaction sa;

    sa.sa_flags = SA_SIGINFO | SA_RESTART | SA_NOCLDWAIT;
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = handleChildTerm;
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
    if (pid == 0) { // child process
        while (*runChildProcess) {
            sigwait(&signalWait, &signalWaitInfo);
            if (*runChildProcess == 0) {
                break;
            }
            // run bigger code here
            printf("Child process started: %d\n", getpid());
        }
        _exit(EXIT_SUCCESS);
    }

    printf("Parent process: %d\n", getpid());

    runChild(pid);
    runChild(pid);
    runChild(pid);
    runChild(pid);
    termChild(pid);

    printf("Done\n");

    exit(EXIT_SUCCESS);
}