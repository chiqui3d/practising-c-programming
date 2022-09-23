#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* A semaphore counting the number of jobs in the queue. */
sem_t job_queue_count;

/* A mutex protecting job_queue. */
pthread_mutex_t job_queue_mutex = PTHREAD_MUTEX_INITIALIZER;

struct job {
    /* Link field for linked list. */
    struct job *next;
    /* Other fields describing work to be done... */
    char name[50];
};
/* A linked list of pending jobs. */
struct job *job_queue;

/* Perform one-time initialization of the job queue. */
void initialize_job_queue() {
    /* The queue is initially empty. */
    job_queue = NULL;
    /* Initialize the semaphore which counts jobs in the queue. Its
    initial value should be zero. */
    sem_init(&job_queue_count, 0, 0);
}

void process_job(struct job *job, long idThread) {
    /* Do the job. */
    printf("%s by thread id=%li\n", job->name, idThread);
    /* Free the job. */
    free(job);
}

/* void enqueue_job(struct job *new_job) {
    pthread_mutex_lock(&job_queue_mutex);

    new_job->next = job_queue;
    job_queue = new_job;
    pthread_mutex_unlock(&job_queue_mutex);
} */

/* Add a new job to the front of the job queue. */
void enqueue_job() {
    struct job *new_job;

    /* Allocate a new job object. */
    new_job = malloc(sizeof(struct job));
    /* Set the other fields of the job struct here... */
    strcpy(new_job->name, "Job name");
    /* Lock the mutex on the job queue before accessing it. */
    pthread_mutex_lock(&job_queue_mutex);
    /* Place the new job at the head of the queue. */
    new_job->next = job_queue;
    job_queue = new_job;
    /* Post to the semaphore to indicate that another job is available. If
    threads are blocked, waiting on the semaphore, one will become
    unblocked so it can process the job. */
    sem_post(&job_queue_count);
    /* Unlock the job queue mutex. */
    pthread_mutex_unlock(&job_queue_mutex);
}

/* Process queued jobs until the queue is empty. */
void *thread_function(void *arg) {

    while (1) {
        struct job *next_job;
        /*
            Wait on the job queue semaphore. If its value is positive,
            indicating that the queue is not empty, decrement the count by 1.
            If the queue is empty, block until a new job is enqueued.
        */
        sem_wait(&job_queue_count);

        /* Lock the mutex on the job queue. */
        pthread_mutex_lock(&job_queue_mutex);
        /*
            Because of the semaphore, we know the queue is not empty. Get
            the next available job.
        */
        next_job = job_queue;
        /* Remove this job from the list. */
        job_queue = job_queue->next;

        /* Unlock the mutex on the job queue because we’re done with the queue for now. */
        pthread_mutex_unlock(&job_queue_mutex);
        /* Carry out the work. */
        process_job(next_job, pthread_self());
    }
    return NULL;
}

int main(void) {

    pthread_t t1, t2;

    initialize_job_queue();
    
    struct job *job = malloc(sizeof(struct job));

    strcpy(job->name, "Carlos");
    job->next = NULL;
    job_queue = job;

    job = malloc(sizeof(struct job));
    strcpy(job->name, "Campos");
    job->next = NULL;
    job_queue->next = job;

    // Print list
     /*  for (struct job *tmp = job_queue; tmp != NULL; tmp = tmp->next)
     {
         printf("%s\n", tmp->name);
     }  */



    // Thanks to mutex we can do the threads in order
    pthread_create(&t1, NULL, thread_function, NULL);
    pthread_create(&t2, NULL, thread_function, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

   /*  enqueue_job(); */

    printf("The end\n");

    return 0;
}