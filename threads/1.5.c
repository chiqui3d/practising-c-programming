#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* function to be executed by the new thread */
void *PrintHello(void *data) {
    int my_data = *(int *)data; /* data received by thread */

    pthread_detach(pthread_self());
    printf("Hello from new thread - got %d\n", my_data);
    pthread_exit(NULL); /* terminate the thread */
}

/* like any C program, program's execution begins in main */
int main(int argc, char *argv[]) {
    int rc;              /* return value                           */
    pthread_t thread_id; /* thread's ID (just an integer)          */
    int t = 11;          /* data passed to the new thread          */

    /* create a new thread that will execute 'PrintHello' */
    rc = pthread_create(&thread_id, NULL, PrintHello, &t);
    if (rc) /* could not create thread */
    {
        printf("ERROR: return code from pthread_create is %d \n", rc);
        exit(1);
    }
    printf("Created new thread (%lu) ... \n", thread_id);

    pthread_exit(NULL); /* terminate the thread */
}
