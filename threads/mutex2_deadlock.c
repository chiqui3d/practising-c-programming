#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// http: // www.mit.edu/people/proven/IAP_2000/dead.html
// https://stackoverflow.com/questions/9258308/how-to-use-pthread-mutex-trylock

#define NUM_THREADS 5
#define LOOPS 10000

pthread_mutex_t demoMutex = PTHREAD_MUTEX_INITIALIZER;
FILE *fp;

void *printHello(void *arg) {
    printf("Thread id= %li\n", (long) arg);
    // muted locked
    if (pthread_mutex_trylock(&demoMutex) == 0) {
        unsigned short iterate;
        fprintf(fp, "Fill by %li:\n", (long) arg);
        for (iterate = 0; iterate < LOOPS; iterate++) {
            fprintf(fp, "%d\n", iterate);
        }

        pthread_mutex_unlock(&demoMutex);
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t arrayOfThreadId[5];
    int iterate,result;
    long arrayThreads[NUM_THREADS];


    fp = fopen("deadlock.txt", "w");

    for (iterate = 0; iterate < NUM_THREADS; iterate++) {
        arrayThreads[iterate] = iterate;
        printf("Creating thread %i\n", iterate);
        result = pthread_create(&arrayOfThreadId[iterate], NULL, printHello, (void*)&arrayOfThreadId[iterate]);
        if (result) {
            printf("Error creating thread\n");
            exit(-1);
        }
    }

    for (iterate = 0; iterate < NUM_THREADS; iterate++)
        pthread_join(arrayOfThreadId[iterate], NULL);

    return 0;
}
