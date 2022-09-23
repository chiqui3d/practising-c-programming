#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

// https://stackoverflow.com/questions/16482575/show-the-list-of-running-processes-in-c

int main() {
    /* The argument list to pass to the "ls" command. */
    char *arg_list[] = {
        "ps", /* argv[0], the name of the program. */
        "-eo",
        "stime,pid,ppid,tty,user,comm,%mem,%cpu,wchan:14,stat,cmd",
        "--sort=-%mem",
        "--forest",
        NULL /* The argument list must end with a NULL. */
    };

    pid_t child_pid;
    /* Duplicate this process. */
    child_pid = fork();
    if (child_pid != 0) {
        waitpid(child_pid, NULL, 0);
    } else {
        execvp("ps", arg_list);
        exit(EXIT_SUCCESS);
    }

    printf("ps done\n");

    return 0;
}