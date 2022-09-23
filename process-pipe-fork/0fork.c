#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

static void handle_sigchld(int signum, siginfo_t *sinfo, void *unused)
{

    printf("child %d exited\n", (int)sinfo->si_pid);
    //waitpid(sinfo->si_pid, NULL, 0);

}

int main() {

    printf("the main program process ID is %d\n", (int)getpid());

    struct sigaction sa;

    sa.sa_flags = SA_SIGINFO | SA_NOCLDWAIT;
    sa.sa_sigaction = &handle_sigchld;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    pid_t c;
    for (int i = 0; i < 5; i++) {
        c = fork();
        if (c == 0) // fork returns 0 to the child process so it enters "if" block
        {
            //
            printf("[son] pid %d from [parent] pid %d\n", getpid(), getppid());
            sleep(i);
            printf("[son] pid %d with [i] %d\n", getpid(), i);
            _exit(0);
        }
    }

    waitpid(-1, NULL, WNOHANG);
 
    /*
    int status;    // status of child process
    pid_t pid;
     while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        printf("child %d terminated\n", pid);
    } */

    printf("end?\n");


    return 0;
}