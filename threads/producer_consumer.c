#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_THREADS_CONSUMERS 10

struct job {
    /* Link field for linked list. */
    struct job *next;
    char name[50];
    int index;
};

/* A linked list of pending jobs. */
struct job *job_queue;

struct failJobs {
    int index;
    char error[CHAR_MAX];
};

struct failJobs *fail_jobs;
int countFailJobs = 0;

/* A mutex protecting job_queue. */
pthread_mutex_t job_queue_mutex = PTHREAD_MUTEX_INITIALIZER;

void *producer_function();
void *consumer_function(void *arg);
void process_hard_job(struct job *job, long idThread);
void listJobs();
void listFailJobs();
void enqueue_job();

int main(void) {

    pthread_t producer;
    pthread_t consumers[MAX_THREADS_CONSUMERS];

    pthread_create(&producer, NULL, producer_function, NULL);

    int iConsumers[MAX_THREADS_CONSUMERS];
    for (int i = 0; i < MAX_THREADS_CONSUMERS; i++) {
        iConsumers[i] = i;
        pthread_create(&consumers[i], NULL, consumer_function, (void *)&iConsumers[i]);
    }

    // enqueue new jobs
    // Add jobs to queue before join the produce
    struct job *job = malloc(sizeof(struct job));
    strcpy(job->name, "Albert");
    job->index = 3;
    enqueue_job(job);

    job = malloc(sizeof(struct job));
    strcpy(job->name, "Mark");
    job->index = 4;
    enqueue_job(job);

    pthread_join(producer, NULL);


    for (int i = 0; i < MAX_THREADS_CONSUMERS; i++) {
        pthread_join(consumers[i], NULL);
    }

    listFailJobs();

    free(fail_jobs);
      // Destroy the mutex since we no longer use it.
    pthread_mutex_destroy(&job_queue_mutex);

    pthread_exit(NULL);

    printf("\nThe end\n");

    return EXIT_SUCCESS;
}

void *producer_function() {

    pthread_mutex_lock(&job_queue_mutex);

    struct job *job = malloc(sizeof(struct job));

    strcpy(job->name, "Charles");
    job->index = 1;
    job->next = NULL;
    job_queue = job;

    job = malloc(sizeof(struct job));

    strcpy(job->name, "Lauren");
    job->index = 2;
    job->next = NULL;
    job_queue->next = job;
    pthread_mutex_unlock(&job_queue_mutex);


    return NULL;
}

/* Process queued jobs until the queue is empty. */
void *consumer_function(void *arg) {

     int i = *(int*)arg;

    while (1) {
        struct job *next_job;
        /* Lock the mutex on the job queue. */
        pthread_mutex_lock(&job_queue_mutex);
        /* Now it’s safe to check if the queue is empty. */

        if (job_queue == NULL){
            next_job = NULL;
        }else {
            /* Get the next available job. */
            next_job = job_queue;
            /* Remove this job from the list. */
            job_queue = job_queue->next;

            if (next_job->index == 2 || next_job->index == 4) {
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
                fail_job.index = next_job->index;
                strcpy(fail_job.error, message);

                fail_jobs[countFailJobs-1] = fail_job;
            }
        }
        /* Unlock the mutex on the job queue because we’re done with the queue for now. */
        pthread_mutex_unlock(&job_queue_mutex);
        
        /* Was the queue empty? If so, end the thread. */
        if (next_job == NULL)
            break;
        
        printf("Nº thread: %i\n",i);
        process_hard_job(next_job, pthread_self());
    }
    return NULL;
}

void process_hard_job(struct job *job, long idThread) {
    /* Do the job. */
    printf("Job %s: index=%i and threadId=%li\n", job->name, job->index, idThread);
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
        printf("Job index %i: %s\n",fail_jobs[i].index, fail_jobs[i].error);
    }
    printf("\n");
    
}

void enqueue_job(struct job *new_job) {
    pthread_mutex_lock(&job_queue_mutex);
    new_job->next = job_queue;
    job_queue = new_job;
    pthread_mutex_unlock(&job_queue_mutex);
}
