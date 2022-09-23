
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "become_daemon.h"

int main(int argc, char *argv[]) {
    //becomeDaemon(0);

    int i;

    FILE *fp = fopen("/tmp/myfile", "w+");
    if (fp == NULL){
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < argc; i++){
        printf("argv[%d]: %s\n", i, argv[i]);
        fprintf(fp, "Hello word with argument %s\n",argv[i]);
    }


    fclose(fp);

    printf("MY_NAME: %s\n", getenv("MY_NAME"));  

    exit(EXIT_SUCCESS);
}