#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

struct sigaction sa; // signal 

volatile short int *runChildProcess;
int shmid; // shared memory id


union semun {
    int val;               /* Value for SETVAL */
    struct semid_ds *buf;  /* Buffer for IPC_STAT, IPC_SET */
    unsigned short *array; /* Array for GETALL, SETALL */
    struct seminfo *__buf; /* Buffer for IPC_INFO
                              (Linux-specific) */
};

int semid; // semaphore id
union semun arg;

static void handleChildTerm(int signum, siginfo_t *sinfo, void *unused) {
    printf("Child %d exited\n", (int)sinfo->si_pid);
}

static void runChild(pid_t pid) {
    arg.val = 1; // Allow child process to continue
    if (semctl(semid, 0, SETVAL, arg) == -1) {
        perror("Semctl run child");
        exit(EXIT_FAILURE);
    }

    // When SIGCHLD is caught, sleep(3) is interrupted and the child process is terminated
    // https://stackoverflow.com/questions/14266485/understanding-sigchld-when-the-child-process-terminates
    unsigned short int sleepTime = 3;
    while ((sleepTime = sleep(sleepTime)) > 0);
}

static void blockChild() {
    arg.val = 0; // Block child process
    if (semctl(semid, 0, SETVAL, arg) == -1) {
        perror("Semctl block child");
        exit(EXIT_FAILURE);
    }
}

static void termChild(pid_t pid) {
    *runChildProcess = 0; // set to 0 to exit the loop
    runChild(pid);                              
   // restore default signal handler
    sa.sa_handler = SIG_DFL;
    sa.sa_flags = 0;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("Sigaction reset SIGCHLD");
        exit(EXIT_FAILURE);
    }
    if (shmctl(shmid, IPC_RMID, NULL) == -1) { // remove shared memory
        perror("Shmctl");
        exit(EXIT_FAILURE);
    }
    if (semctl(semid, 0, IPC_RMID) == -1) { // remove semaphore
        perror("Semctl");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {

    // shared memory
    shmid = shmget(IPC_PRIVATE, sizeof *runChildProcess, IPC_CREAT | 0600);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }
    runChildProcess = shmat(shmid, NULL, 0); // set to 1 to run the loop
    if (runChildProcess == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    *runChildProcess = 1;

    // semaphore
    semid = semget(IPC_PRIVATE, 1, IPC_CREAT | 0600);
    blockChild();

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
        struct sembuf sop;
        sop.sem_num = 0; /* Operate on semaphore 0 */
        sop.sem_op = -1;  /* Wait for value to equal 0 */
        sop.sem_flg = 0;
        while (*runChildProcess) {
            if (semop(semid, &sop, 1) == -1) {
                perror("semop");
                exit(EXIT_FAILURE);
            }
            if (*runChildProcess == 0) {
                break;
            }
            // run bigger code here
            printf("Child process started: %d\n", getpid());
            blockChild();
        }
        _exit(EXIT_SUCCESS);
    }

    printf("runChildProcess: %d\n", *runChildProcess);
    printf("Parent process: %d\n", getpid());

    runChild(pid);
    runChild(pid);
    runChild(pid);
    termChild(pid);

    printf("runChildProcess: %d\n", *runChildProcess);
    printf("Done\n");

    exit(EXIT_SUCCESS);
}