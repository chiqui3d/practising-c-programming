#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

pthread_mutex_t lock;
int j;

struct job {
    /* Link field for linked list. */
    struct job *next;
    /* Other fields describing work to be done... */
    char name[50];
};
/* A linked list of pending jobs. */
struct job *job_queue;
/* A mutex protecting job_queue. */
pthread_mutex_t job_queue_mutex = PTHREAD_MUTEX_INITIALIZER;

void process_job(struct job *job, long idThread) {
    /* Do the job. */
    printf("%s by thread id=%li\n", job->name, idThread);
    /* Free the job. */
    free(job);
}

/* Process queued jobs until the queue is empty. */
void *thread_function(void *arg) {
    long idThread;
    while (1) {
        struct job *next_job;
        /* Lock the mutex on the job queue. */
        pthread_mutex_lock(&job_queue_mutex);
        /* Now it’s safe to check if the queue is empty. */
        
        if (job_queue == NULL)
            next_job = NULL;
        else {
            /* Get the next available job. */
            next_job = job_queue;
            /* Remove this job from the list. */
            job_queue = job_queue->next;
        }
        /* Unlock the mutex on the job queue because we’re done with the queue for now. */
        pthread_mutex_unlock(&job_queue_mutex);
        /* Was the queue empty? If so, end the thread. */
        if (next_job == NULL)
            break;

         idThread = pthread_self();
        /* Carry out the work. */
        process_job(next_job, idThread);
    }
    return NULL;
}

void enqueue_job(struct job *new_job) {
    pthread_mutex_lock(&job_queue_mutex);

    new_job->next = job_queue;
    job_queue = new_job;
    pthread_mutex_unlock(&job_queue_mutex);
}

int main(void) {

    pthread_t t1, t2;
    struct job *job =  malloc(sizeof(struct job));

    strcpy(job->name, "Carlos");
    job->next = NULL;
    job_queue = job;
   
    
    job =  malloc(sizeof(struct job)); 
    strcpy(job->name, "Campos");
    job->next = NULL;
    job_queue->next = job;

     // Print list
   /*  for (struct job *tmp = job_queue; tmp != NULL; tmp = tmp->next)
    {
        printf("%s\n", tmp->name);
    } */


    // Thanks to mutex we can do the threads in order
    pthread_create(&t1, NULL, thread_function, NULL);
    pthread_create(&t2, NULL, thread_function, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}