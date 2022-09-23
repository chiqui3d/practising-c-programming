#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    char *newArgv[] = {"myecho", "hello", "world", NULL};
    char *newEnviron[] = {"MY_NAME=chiqui3d",NULL};

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid == 0)
    {
        execve("./process-pipe-fork/execve/myecho", newArgv, newEnviron);
        /* The execve function returns only if an error occurs. */
        perror("An error occurred in execve");
        abort();
    }

    int status;    // status of child process
    wait(&status);

    if (WIFEXITED(status)){
        printf("the child process exited normally, with exit code %d\n", WEXITSTATUS(status));
    }else{
        printf("the child process exited abnormally\n");
    }

    printf("done with main program\n");
    
    exit(EXIT_SUCCESS);
}