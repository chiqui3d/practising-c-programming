#include <errno.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h> 

struct sigaction sa; // signal

short int *runChildProcess; // mmap shared memory

sem_t *sem; // semaphore

static void handleChildTerm(int signum, siginfo_t *sinfo, void *unused) {
    printf("Child %d exited\n", (int)sinfo->si_pid);
}

static void runChild(pid_t pid) {
    if (sem_post(sem) == -1) {
        perror("sem_post");
        exit(EXIT_FAILURE);
    }
    // When SIGCHLD is caught, sleep(3) is interrupted
    // https://stackoverflow.com/questions/14266485/understanding-sigchld-when-the-child-process-terminates
    unsigned short int sleepTime = 3;
    while ((sleepTime = sleep(sleepTime)) > 0)
        ;
}

static void termChild(pid_t pid) {
    *runChildProcess = 0; // set to 0 to exit the loop
    runChild(pid);
    signal(SIGCHLD, SIG_DFL);  // restore default signal handler
    if(munmap(runChildProcess, sizeof *runChildProcess) == -1) {
        perror("Munmap");
        exit(EXIT_FAILURE);
    }
    if (sem_destroy(sem) == -1) {
        perror("sem_destroy");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {

    setbuf(stdout, NULL); // make stdout unbuffered

    // mmap: memory mapping file, shared with child process with the flag MAP_ANONYMOUS | MAP_SHARED
    runChildProcess = mmap(NULL, sizeof *runChildProcess, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    if (runChildProcess == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
    *runChildProcess = 1;

    // semaphore
    sem = mmap(0, sizeof(*sem), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (sem_init(sem, 1 /*shared*/, 0 /*init val*/) == -1) {
        perror("sem_init");
        exit(EXIT_FAILURE);
    }

    // signal handler for SIGCHLD (exit child process)
    sa.sa_flags = SA_SIGINFO | SA_RESTART | SA_NOCLDWAIT;
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = handleChildTerm;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction SIGCHLD");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) { // child process
        char rootChildPidParam[10];
        snprintf(rootChildPidParam, 10, "%d", getpid());

        char *childArgv[] = {"process-pipe-fork/execve/myecho", "hello", "world", rootChildPidParam, NULL};
        char *childEnviron[] = {"MY_NAME=jcarlosweb", NULL};
        printf("Root Child started: %s\n", rootChildPidParam);

        while (*runChildProcess) {
            sem_wait(sem);
            // check again
            if (*runChildProcess == 0) {
                break;
            }
            pid_t leafChildPid = fork();
            if (leafChildPid == -1) {
                perror("fork sibling");
                _exit(EXIT_FAILURE);
            }
            if (leafChildPid == 0) { // child process
                printf("Leaf process with execve started: %d\n", getpid());
                // TODO: make child process in background
                // setsid();
                // freopen("/dev/null", "w", stdout);
                // freopen("/dev/null", "w", stderr); 
                execve("process-pipe-fork/execve/myecho", childArgv, childEnviron);
                perror("execve myecho");
                _exit(EXIT_FAILURE);
            }
           
        }
        _exit(EXIT_SUCCESS);
    }

    printf("Parent process: %d\n", getpid());

    runChild(pid);
    runChild(pid);
    runChild(pid);
    
    termChild(pid);

    printf("Done\n");

    exit(EXIT_SUCCESS);
}