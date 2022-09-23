
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

void *thread_main(void *arg) {
    long rank = (long)arg;
    printf("Hello from thread %ld\n", rank);
    return (void *)(rank + 1);
    // pthread_exit((void*) rank+1) could also be used equivalently
}
int main(int argc, char **argv) {
    long n_threads = 5;
    pthread_t *vth = (pthread_t *)malloc(sizeof(pthread_t) * (n_threads));
    for (long rank = 0; rank < n_threads; rank++) {
        pthread_create(&vth[rank], NULL, thread_main, (void *)rank);
    }
    printf("Hello from main thread\n");
    for (long rank = 0; rank < n_threads; rank++) {
        long rval;
        pthread_join(vth[rank], (void **)&rval);
        printf("Thread %ld done, returned %ld\n", rank, rval);
    }
    free(vth);
    printf("Done\n");
    return 0;
}