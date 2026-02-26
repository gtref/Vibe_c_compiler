/**
 * The thread pool implementation has been hardened to enforce a maximum thread limit and a maximum job queue size.
 * These changes, along with shutdown-aware job rejection, mitigate resource exhaustion and prevent memory leaks or race conditions.
 * This code is AI-generated.
 */
#ifndef VIBE_THREAD_POOL_H
#define VIBE_THREAD_POOL_H

/* SENTINEL: Maximum number of jobs allowed in the queue to prevent DoS via memory exhaustion */
#define VIBE_THREAD_POOL_MAX_QUEUE_SIZE 65536

#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>

/* Structure representing a single job to be executed by the thread pool */
typedef struct vibe_job {
    void (*function)(void* arg); /* Function pointer to the task */
    void* arg;                   /* Argument to be passed to the function */
    struct vibe_job* next;       /* Pointer to the next job in the queue */
} vibe_job_t;

/* Main thread pool structure managing workers and the job queue */
typedef struct {
    pthread_mutex_t lock;        /* Mutex for protecting access to the job queue */
    pthread_cond_t notify;       /* Condition variable to signal workers when a job is available */
    pthread_t* threads;          /* Array of worker thread IDs */
    vibe_job_t* queue_head;      /* Pointer to the first job in the queue */
    vibe_job_t* queue_tail;      /* BOLT: Pointer to the last job in the queue for O(1) insertion */
    int thread_count;            /* Total number of worker threads */
    int queue_size;              /* Current number of jobs in the queue */
    bool shutdown;               /* Flag indicating if the pool is shutting down */
} vibe_thread_pool_t;

/* Internal worker function that threads execute to pull and run jobs from the queue */
static void* vibe_worker(void* thread_pool) {
    vibe_thread_pool_t* pool = (vibe_thread_pool_t*)thread_pool;
    while (true) {
        pthread_mutex_lock(&(pool->lock));
        /* Wait for a job to be added or for the pool to shut down */
        while (pool->queue_size == 0 && !pool->shutdown) {
            pthread_cond_wait(&(pool->notify), &(pool->lock));
        }
        /* Exit if the pool is shutting down */
        if (pool->shutdown) {
            pthread_mutex_unlock(&(pool->lock));
            pthread_exit(NULL);
        }
        /* Retrieve the next job from the head of the queue */
        vibe_job_t* job = pool->queue_head;
        pool->queue_head = job->next;
        /* BOLT: Update tail if the queue is now empty to maintain O(1) insertion property */
        if (pool->queue_head == NULL) {
            pool->queue_tail = NULL;
        }
        pool->queue_size--;
        pthread_mutex_unlock(&(pool->lock));

        /* Execute the job and free the job structure */
        (*(job->function))(job->arg);
        free(job);
    }
    return NULL;
}

/* Initialize the thread pool with a specified number of worker threads */
static inline vibe_thread_pool_t* vibe_thread_pool_create(int num_threads) {
    /* Sentinel: Enforce a reasonable thread limit to prevent resource exhaustion and integer overflow */
    if (num_threads <= 0 || num_threads > 1024) return NULL;
    vibe_thread_pool_t* pool = (vibe_thread_pool_t*)malloc(sizeof(vibe_thread_pool_t));
    if (!pool) return NULL;

    pool->thread_count = num_threads;
    pool->queue_size = 0;
    pool->queue_head = NULL;
    pool->queue_tail = NULL; /* BOLT: Initialize tail pointer for O(1) job queueing */
    pool->shutdown = false;

    /* Initialize synchronization primitives */
    if (pthread_mutex_init(&(pool->lock), NULL) != 0) {
        free(pool);
        return NULL;
    }
    if (pthread_cond_init(&(pool->notify), NULL) != 0) {
        pthread_mutex_destroy(&(pool->lock));
        free(pool);
        return NULL;
    }

    /* Allocate memory for thread tracking */
    pool->threads = (pthread_t*)malloc(sizeof(pthread_t) * num_threads);
    if (!pool->threads) {
        pthread_cond_destroy(&(pool->notify));
        pthread_mutex_destroy(&(pool->lock));
        free(pool);
        return NULL;
    }

    /* Create the requested number of worker threads */
    int created = 0;
    for (int i = 0; i < num_threads; i++) {
        if (pthread_create(&(pool->threads[i]), NULL, vibe_worker, (void*)pool) != 0) {
            /* If thread creation fails, trigger pool shutdown and cleanup already created threads */
            pool->shutdown = true;
            pthread_mutex_lock(&(pool->lock));
            pthread_cond_broadcast(&(pool->notify));
            pthread_mutex_unlock(&(pool->lock));

            for (int j = 0; j < created; j++) {
                pthread_join(pool->threads[j], NULL);
            }
            free(pool->threads);
            pthread_cond_destroy(&(pool->notify));
            pthread_mutex_destroy(&(pool->lock));
            free(pool);
            return NULL;
        }
        created++;
    }
    return pool;
}

/* Enqueue a new job to be executed by one of the worker threads in the pool */
static inline void vibe_thread_pool_add_job(vibe_thread_pool_t* pool, void (*function)(void*), void* arg) {
    /* Validate input parameters to prevent NULL pointer dereferences */
    if (!pool || !function) return;

    vibe_job_t* job = (vibe_job_t*)malloc(sizeof(vibe_job_t));
    if (!job) return;

    job->function = function;
    job->arg = arg;
    job->next = NULL;

    pthread_mutex_lock(&(pool->lock));
    /* Sentinel: Reject new jobs if the pool is shutting down or full to prevent resource leakage/exhaustion */
    if (pool->shutdown || pool->queue_size >= VIBE_THREAD_POOL_MAX_QUEUE_SIZE) {
        pthread_mutex_unlock(&(pool->lock));
        free(job);
        return;
    }
    /* BOLT: Efficient O(1) job insertion using tail pointer to minimize lock contention */
    if (pool->queue_tail == NULL) {
        pool->queue_head = job;
        pool->queue_tail = job;
    } else {
        pool->queue_tail->next = job;
        pool->queue_tail = job;
    }
    pool->queue_size++;
    /* Signal a worker that a new job is available */
    pthread_cond_signal(&(pool->notify));
    pthread_mutex_unlock(&(pool->lock));
}

/**
 * vibe_thread_pool_destroy - Safely shuts down the pool and frees all associated resources
 */
static inline void vibe_thread_pool_destroy(vibe_thread_pool_t* pool) {
    if (!pool) return;

    /* Signal all worker threads to shut down and wake them up */
    pthread_mutex_lock(&(pool->lock));
    pool->shutdown = true;
    pthread_cond_broadcast(&(pool->notify));
    pthread_mutex_unlock(&(pool->lock));

    /* Wait for all worker threads to finish execution */
    for (int i = 0; i < pool->thread_count; i++) {
        pthread_join(pool->threads[i], NULL);
    }

    /* Free all pending jobs in the queue to prevent memory leaks */
    vibe_job_t* curr = pool->queue_head;
    while (curr) {
        vibe_job_t* next = curr->next;
        free(curr);
        curr = next;
    }

    /* Clean up synchronization primitives and allocated memory */
    free(pool->threads);
    pthread_mutex_destroy(&(pool->lock));
    pthread_cond_destroy(&(pool->notify));
    free(pool);
}

#endif
