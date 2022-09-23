#include "../include/h_error.h"
#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h> // for sigaction
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAX_THREADS_CONSUMERS 10
#define SECONDS_SEMAPHORE_WAIT 10

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

void *producer_function();
void *consumer_function(void *arg);
void process_hard_job(struct job *job, long idThread);
void enqueue_job(char name[50], int id);
void listJobs();
void listFailJobs();

/* Perform one-time initialization of the job queue. */
void initialize_job_queue() {
    /* The queue is initially empty. */
    job_queue = NULL;
    /* Initialize the semaphore which counts jobs in the queue. Its
    initial value should be zero. */
    sem_init(&job_queue_count, 0, 0);
}

static void handler(int sig) {
    write(STDOUT_FILENO, "sem_post() from handler\n", 24);
    if (sem_post(&job_queue_count) == -1) {
        write(STDERR_FILENO, "sem_post() failed\n", 18);
        _exit(EXIT_FAILURE);
    }
}

int main(void) {

    pthread_t producer;
    pthread_t consumers[MAX_THREADS_CONSUMERS];

    struct sigaction action = {};

    initialize_job_queue();

    /* Establish SIGALRM handler; set alarm timer using argv[1] */

    /*  action.sa_handler = handler;
     sigemptyset(&action.sa_mask);
     action.sa_flags = 0;
     if (sigaction(SIGALRM, &action, NULL) == -1) {
         die("sigaction");
     }

     alarm(4); */

    pthread_create(&producer, NULL, producer_function, NULL);

    int iConsumers[MAX_THREADS_CONSUMERS];
    for (int i = 0; i < MAX_THREADS_CONSUMERS; i++) {
        iConsumers[i] = i;
        pthread_create(&consumers[i], NULL, consumer_function, (void *)&iConsumers[i]);
    }

    // enqueue new jobs
    // Add jobs to queue before join the produce
    sleep(2);
    enqueue_job("Albert", 3);
    enqueue_job("Mark", 4);

    pthread_join(producer, NULL);

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

    enqueue_job("Charles", 1);
    enqueue_job("Lauren", 2);

    return NULL;
}

/* Process queued jobs until the queue is empty. */
void *consumer_function(void *arg) {
    int idConsumer = *(int *)arg;
    struct job *next_job;
    struct timespec ts;
    int s;
    while (1) {
        /* Calculate relative interval as current time plus number of seconds given */
        if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
            die("clock_gettime");
        }
        ts.tv_sec += SECONDS_SEMAPHORE_WAIT;

        // https://stackoverflow.com/questions/52331735/understanding-sem-timedwait
        while ((s = sem_timedwait(&job_queue_count, &ts)) == -1 && errno == EINTR) {
            continue; // Restart if interrupted by signal
        }

        if (s < 0) {
            if (errno == ETIMEDOUT) {
                printf("Consumer %i waited more than %i seconds for data.\n", idConsumer, SECONDS_SEMAPHORE_WAIT);
                break;
            }
            printf("Consumer %i received an error: %s.\n", idConsumer, strerror(errno));
            break;
        }
        /* Restart if interrupted by handler */
        /* Lock the mutex on the job queue. */
        pthread_mutex_lock(&job_queue_mutex);
        /* Now it’s safe to check if the queue is empty. */

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

        printf("Nº Consumer: %i\n", idConsumer);
        process_hard_job(next_job, pthread_self());
    }
    return NULL;
}

void process_hard_job(struct job *job, long idThread) {
    /* Do the job. */
    printf("Job %s: id=%i and threadId=%li\n", job->name, job->id, idThread);
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
        printf("Job id %i: %s\n", fail_jobs[i].id, fail_jobs[i].error);
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