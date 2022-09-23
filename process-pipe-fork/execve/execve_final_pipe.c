#include <errno.h>     // for errno
#include <signal.h>    // sigaction
#include <stdbool.h>   // bool
#include <stdio.h>     // printf
#include <stdlib.h>    // exit
#include <string.h>    // strlen
#include <sys/mman.h>  // mmap
#include <sys/types.h> // pid_t
#include <unistd.h>    // fork

bool *runChildProcess; // mmap shared memory
int fd[2]; // pipe

static void runTermChildHandler(int signum, siginfo_t *sinfo, void *unused) {
    *runChildProcess = 0;
}
static void termChildHandler(int signum, siginfo_t *sinfo, void *unused) {
    printf("Child %d exited\n", (int)sinfo->si_pid);
}

static void runChild(int pipeFd, char *buffer) {

    if (write(pipeFd, buffer, strlen(buffer) + 1) == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }
    // When SIGCHLD is caught, sleep(3) is interrupted
    // https://stackoverflow.com/questions/14266485/understanding-sigchld-when-the-child-process-terminates
    unsigned short int sleepTime = 3;
    while ((sleepTime = sleep(sleepTime)) > 0)
        ;
}

static void termChild(pid_t pid, int pipeFd) {

    kill(pid, SIGUSR1);
    close(pipeFd);
    unsigned short int sleepTime = 1;
    while ((sleepTime = sleep(sleepTime)) > 0)
        ;
    signal(SIGUSR1, SIG_DFL); // restore default signal handle
    signal(SIGCHLD, SIG_DFL); // restore default signal handle
    munmap(runChildProcess, sizeof *runChildProcess);
}

int main(int argc, char *argv[]) {

    setbuf(stdout, NULL); // make stdout unbuffered

    // mmap: memory mapping file, shared with child process with the flag MAP_ANONYMOUS | MAP_SHARED
    runChildProcess = mmap(NULL, sizeof *runChildProcess, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    if (runChildProcess == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
    *runChildProcess = 1; // set to 1 to run the loop

    struct sigaction sa;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_sigaction = runTermChildHandler;
    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction SIGUSR1");
        exit(EXIT_FAILURE);
    }
    sa.sa_flags = SA_RESTART | SA_SIGINFO | SA_NOCLDSTOP;
    sa.sa_sigaction = termChildHandler;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction SIGCHLD");
        exit(EXIT_FAILURE);
    }

    if (pipe(fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    char buffer[100];

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        close(fd[1]); // close write end of pipe
        printf("Child process: %d\n", getpid());
        while (*runChildProcess) {
            ssize_t nbytes = read(fd[0], buffer, sizeof(buffer)); // block until data is available from parent process
            if (nbytes == -1) {                 // EINTR: interrupted by a signal, although SA_RESTART is set no is no need to check for EINTR
                if (errno == EINTR) { 
                   continue;
                }else{
                    perror("read");
                    exit(EXIT_FAILURE);
                }
            }
            if (nbytes == 0) {
                continue;
            }
            printf("Buffer: %s\n", buffer);
        }
        close(fd[0]); // close read end of pipe
        _exit(EXIT_SUCCESS);
    }

    printf("Parent process: %d\n", getpid());
    close(fd[0]); // close read end of pipe

    runChild(fd[1], "Thanks Nate Eldredge");
    runChild(fd[1], "Thanks Luis Colorado");
    runChild(fd[1], "Thanks www.stackoverflow.com");
    termChild(pid, fd[1]);

    printf("Done\n");

    exit(EXIT_SUCCESS);
}