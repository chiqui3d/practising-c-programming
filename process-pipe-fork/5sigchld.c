//  Cleaning Up Children by Handling SIGCHLD

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

sig_atomic_t child_exit_status;

void clean_up_child_process(int signal_number) {
    /* Clean up the child process. */
    int status;
    wait(&status);
    /* Store its exit status in a global variable. */
    child_exit_status = status;
    printf("the child process exited with signal_number %d\n", status);
}
int main() {
    /* Handle SIGCHLD by calling clean_up_child_process. */
    struct sigaction sigchld_action = {};
    memset(&sigchld_action, 0, sizeof(sigchld_action));
    sigchld_action.sa_handler = &clean_up_child_process;
    sigaction(SIGCHLD, &sigchld_action, NULL);
    /* Now do things, including forking a child process. */

    pid_t child_pid = fork();
    if (child_pid > 0) {
        printf("Parent is going to sleep for 10 secs.\n");
        waitpid(child_pid, NULL, 0); // WNOHANG not-blocking
        printf("Parent woke up!\n");
    } else {
        printf("A child is born with no state of mind, "
               "blind to the ways of mankind.\n");
        sleep(2);
        printf("It was plain to see that your life was lost, "
               "you was cold and your body swung back and fort.\n");

    }


    return 0;
}
