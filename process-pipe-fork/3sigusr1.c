#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

sig_atomic_t sigusr1_count = 0;

void handler(int signal_number) {
    ++sigusr1_count;
}

int main() {
    struct sigaction sa = {};
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &handler;
    sigaction(SIGUSR1, &sa, NULL);
    // SIGUSR1: user-defined signal number 1, which is used to send a signal to a process.

     pid_t child_pid;
    /* Duplicate this process. */
    child_pid = fork();
    if (child_pid != 0){
        printf("Message: %s\n","Hello, here parent");
        kill(child_pid, SIGUSR1);
        exit(EXIT_SUCCESS);
    }else{
        char message[] = "Hello, here children.";
        printf("Message: %s\n",message);
        kill((int)getpid(), SIGUSR1);
    }

    printf("SIGUSR1 was raised %d times\n", sigusr1_count);
    return 0;
}