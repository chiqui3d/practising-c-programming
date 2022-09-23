#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define SIZE 512


// https://www.programacion.com.py/escritorio/c/pipes-en-c-linux
// bidireccional with 2 pipes
int main(int argc, char **argv) {
    pid_t pid;
    int a[2], b[2], readbytes;
    char buffer[SIZE];

    pipe(a);
    pipe(b);

    if ((pid = fork()) == 0) { // hijo
        close(a[1]);           /* cerramos el lado de escritura del pipe */
        close(b[0]);           /* cerramos el lado de lectura del pipe */

        while ((readbytes = read(a[0], buffer, SIZE)) > 0)
            write(1, buffer, readbytes);
        close(a[0]);

        strcpy(buffer, "Soy tu hijo hablándote por la otra tubería.\n");
        write(b[1], buffer, strlen(buffer));
        char hijoPadre[] = "Yo soy tu padre\n";
        write(b[1], hijoPadre, strlen(hijoPadre));
        
        close(b[1]);
    } else {         // padre
        close(a[0]); /* cerramos el lado de lectura del pipe */
        close(b[1]); /* cerramos el lado de escritura del pipe */

        strcpy(buffer, "Soy tu padre hablándote por una tubería.\n");
        write(a[1], buffer, strlen(buffer));
        close(a[1]);

        while ((readbytes = read(b[0], buffer, SIZE)) > 0)
            write(1, buffer, readbytes);
        close(b[0]);
    }
    waitpid(pid, NULL, 0);
    exit(0);
}