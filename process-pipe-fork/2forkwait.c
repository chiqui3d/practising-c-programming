// C program to demonstrate working of wait()
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    pid_t child_pid;
    child_pid = fork();

    if (child_pid == 0){
        printf("HC: hello from child\n");
        exit(0);
    }else {
        printf("HP: hello from parent\n");
        wait(NULL);
        printf("CT: child has terminated\n");
    }

    printf("Bye\n");
    return 0;
}