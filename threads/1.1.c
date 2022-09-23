#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *say_hi(void *args);
/*
 * Every thread executes say_hi.
 * The argument contains the thread id. */

int main(int argc, char *argv[]) {
    printf("How many threads ? ");
    int n_threads;
    scanf("%d", &n_threads);
    {
        pthread_t t[n_threads];
        pthread_attr_t a;
        int i, id[n_threads];
        printf("creating %d threads ...\n", n_threads);
        for (i = 0; i < n_threads; i++) {
            // In order to avoid sharing data between threads, 
            // To each thread we pass its unique identification label. 
            // To say_hi we pass the address of the label. With the array id[n] we have n distinct addresses:
            // Passing &i instead of &id[i] gives to every thread the same address, and thus the same identification label. 

            id[i] = i;
            pthread_attr_init(&a);
            pthread_create(&t[i], &a, say_hi, (void *)&id[i]);
        }

        printf("waiting for threads to return ...\n");
        /* wait for all threads to complete */
        for (i = 0; i < n_threads; i++) {
            pthread_join(t[i], NULL);
        }
        printf("all threads returned\n");
    }
    return 0;
}

void *say_hi(void *args) {
    int *i = (int *)args;
    printf("hello world from thread %d!\n", *i);
    return NULL;
}