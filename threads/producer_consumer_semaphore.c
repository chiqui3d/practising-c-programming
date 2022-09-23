#include <limits.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define MAX_THREADS_CONSUMERS 10

/* A semaphore counting the number of jobs in the queue. */
sem_t job_queue_count;

/* A mutex protecting job_queue. */
pthread_mutex_t job_queue_mutex = PTHREAD_MUTEX_INITIALIZER;

struct job {
    /* Link field for linked list. */
    struct job *next;
    char name[50];
    int id;
};

/* A linked list of pending jobs. */
struct job *job_queue;

struct failJobs {
    int id;
    char error[CHAR_MAX];
};

struct failJobs *fail_jobs;
int countFailJobs = 0;

/* Perform one-time initialization of the job queue. */
void initialize_job_queue() {
    /* The queue is initially empty. */
    job_queue = NULL;
    /* Initialize the semaphore which counts jobs in the queue. Its
    initial value should be zero. */
    sem_init(&job_queue_count, 0, 0);
}

void *producer_function();
void *consumer_function(void *arg);
void process_hard_job(struct job *job, long idThread);
void enqueue_job(char name[50], int id);
void listJobs();
void listFailJobs();

int main(void) {

    pthread_t producer;
    pthread_t consumers[MAX_THREADS_CONSUMERS];

   initialize_job_queue();

    pthread_create(&producer, NULL, producer_function, NULL);

    int iConsumers[MAX_THREADS_CONSUMERS];
    for (int i = 0; i < MAX_THREADS_CONSUMERS; i++) {
        iConsumers[i] = i;
        pthread_create(&consumers[i], NULL, consumer_function, (void *)&iConsumers[i]);
    }
    
    pthread_join(producer, NULL);

    // Add jobs to queue before join the produce
    sleep(2);
    enqueue_job("Albert",3);
    enqueue_job("Mark",4);

    for (int i = 0; i < MAX_THREADS_CONSUMERS; i++) {
        pthread_join(consumers[i], NULL);
    }

    listFailJobs();

    free(fail_jobs);

    pthread_exit(NULL);

    printf("\nThe end\n");

    return EXIT_SUCCESS;
}

void *producer_function() {

    enqueue_job("Charles",1);
    enqueue_job("Lauren",2);

    return NULL;
}

/* Process queued jobs until the queue is empty. */
void *consumer_function(void *arg) {

    while (1) {
        struct job *next_job;
        /*
            Wait on the job queue semaphore. If its value is positive,
            indicating that the queue is not empty, decrement the count by 1.
            If the queue is empty, block until a new job is enqueued.
        */
        if(sem_wait(&job_queue_count)){
            perror("sem_wait");
            exit(EXIT_FAILURE);
        }

        /* Lock the mutex on the job queue. */
        pthread_mutex_lock(&job_queue_mutex);
        /* Now it’s safe to check if the queue is empty. */

        int i = *(int *)arg;
        next_job = job_queue;
        /* Remove this job from the list. */
        job_queue = job_queue->next;

        if (next_job->id == 2 || next_job->id == 4) {
            countFailJobs++;
            struct failJobs *tmp = realloc(fail_jobs, countFailJobs * sizeof(struct failJobs));
            if (tmp == NULL) {
                if (fail_jobs) {
                    free(fail_jobs);
                }
                perror("Realloc");
                exit(EXIT_FAILURE);
            }

            fail_jobs = tmp;

            char message[CHAR_MAX];
            sprintf(message, "Error in the Thread %li", pthread_self());
            struct failJobs fail_job;
            fail_job.id = next_job->id;
            strcpy(fail_job.error, message);

            fail_jobs[countFailJobs - 1] = fail_job;
        }

        /* Unlock the mutex on the job queue because we’re done with the queue for now. */
        pthread_mutex_unlock(&job_queue_mutex);

        printf("Nº thread: %i\n", i);
        process_hard_job(next_job, pthread_self());
    }
    return NULL;
}

void process_hard_job(struct job *job, long idThread) {
    /* Do the job. */
    printf("Job %s: index=%i and threadId=%li\n", job->name, job->id, idThread);
    /* Free the job. */
    free(job);
}

void listJobs() {
    struct job *job = job_queue;
    while (job != NULL) {
        printf("%s\n", job->name);
        job = job->next;
    }
}

void listFailJobs() {

    printf("\nFail Jobs (%i):\n", countFailJobs);
    int i;
    for (i = 0; i < countFailJobs; i++) {
        printf("Job index %i: %s\n", fail_jobs[i].id, fail_jobs[i].error);
    }
    printf("\n");
}

/* Add a new job to the front of the job queue. */
void enqueue_job(char name[50], int id) {
    struct job *new_job;

    /* Allocate a new job object. */
    new_job = malloc(sizeof(struct job));
    /* Set the other fields of the job struct here... */
    strcpy(new_job->name, name);
    new_job->id = id;

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