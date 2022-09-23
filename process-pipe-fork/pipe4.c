// https://www.geeksforgeeks.org/pipe-system-call/?ref=lbp
// C program to illustrate
// pipe system call in C
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define MSGSIZE 16
char *msg1 = "hello, world #1";
char *msg2 = "hello, world #2";
char *msg3 = "hello, world #3";

int main() {
    char buffer[MSGSIZE];
    int myPipe[2], i;


    if (pipe(myPipe) == -1) {
        fprintf(stderr, "Pipe failed.\n");
        return EXIT_FAILURE;
    }
    /* continued */
    /* write pipe */

    write(myPipe[1], msg3, MSGSIZE);
    write(myPipe[1], msg1, MSGSIZE);
    write(myPipe[1], msg2, MSGSIZE);
    close(myPipe[1]);

    /* for (i = 0; i < 3; i++) {
        read(myPipe[0], buffer, MSGSIZE);
        printf("% s\n", buffer);
    }*/

    while(read(myPipe[0], buffer, MSGSIZE) > 0) {
        printf("%s\n", buffer);
    }

    close(myPipe[0]);

    return EXIT_SUCCESS;
}