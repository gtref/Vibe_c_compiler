/*
 * This header has been updated to include comprehensive documentation and logic explanations.
 * The changes enhance maintainability by detailing the high-performance worker thread pool and its concurrency management.
 * THIS CODE IS AI GENERATED.
 */

#ifndef VIBE_THREAD_POOL_H
#define VIBE_THREAD_POOL_H

#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * vibe_job_t - Represents a single task in the thread pool queue.
 */
typedef struct vibe_job {
    void (*function)(void* arg); /* Pointer to the task function */
    void* arg;                   /* Argument to pass to the function */
    struct vibe_job* next;       /* Pointer to the next job in the queue */
} vibe_job_t;

/**
 * vibe_thread_pool_t - Manages a set of worker threads and a job queue.
 */
typedef struct {
    pthread_mutex_t lock;    /* Mutex for thread-safe access to the queue */
    pthread_cond_t notify;   /* Condition variable for worker notification */
    pthread_t* threads;      /* Array of worker threads */
    vibe_job_t* queue_head;  /* Head of the job queue */
    vibe_job_t* queue_tail;  /* Tail of the job queue (for O(1) insertion) */
    int thread_count;        /* Number of worker threads */
    int queue_size;          /* Current number of jobs in the queue */
    bool shutdown;           /* Flag indicating if the pool is shutting down */
} vibe_thread_pool_t;

/**
 * vibe_worker - The main loop for each worker thread.
 * @thread_pool: Pointer to the vibe_thread_pool_t structure
 *
 * Workers wait for new jobs on the condition variable, execute them,
 * and then free the job structure.
 */
static void* vibe_worker(void* thread_pool) {
    vibe_thread_pool_t* pool = (vibe_thread_pool_t*)thread_pool;
    while (true) {
        pthread_mutex_lock(&(pool->lock));
        while (pool->queue_size == 0 && !pool->shutdown) {
            pthread_cond_wait(&(pool->notify), &(pool->lock));
        }
        if (pool->shutdown) {
            pthread_mutex_unlock(&(pool->lock));
            pthread_exit(NULL);
        }
        vibe_job_t* job = pool->queue_head;
        pool->queue_head = job->next;
        // BOLT: Update tail if the queue is now empty
        if (pool->queue_head == NULL) {
            pool->queue_tail = NULL;
        }
        pool->queue_size--;
        pthread_mutex_unlock(&(pool->lock));
        (*(job->function))(job->arg);
        free(job);
    }
    return NULL;
}

/**
 * vibe_thread_pool_create - Initializes a new worker thread pool.
 * @num_threads: Number of threads to spawn
 *
 * Allocates the pool structure, initializes mutexes/cond vars, and starts workers.
 * Returns the pool pointer or NULL on failure.
 */
static inline vibe_thread_pool_t* vibe_thread_pool_create(int num_threads) {
    vibe_thread_pool_t* pool = (vibe_thread_pool_t*)malloc(sizeof(vibe_thread_pool_t));
    if (!pool) return NULL;

    pool->thread_count = num_threads;
    pool->queue_size = 0;
    pool->queue_head = NULL;
    pool->queue_tail = NULL; // BOLT: Initialize tail pointer
    pool->shutdown = false;

    if (pthread_mutex_init(&(pool->lock), NULL) != 0) {
        free(pool);
        return NULL;
    }
    if (pthread_cond_init(&(pool->notify), NULL) != 0) {
        pthread_mutex_destroy(&(pool->lock));
        free(pool);
        return NULL;
    }

    pool->threads = (pthread_t*)malloc(sizeof(pthread_t) * num_threads);
    if (!pool->threads) {
        pthread_cond_destroy(&(pool->notify));
        pthread_mutex_destroy(&(pool->lock));
        free(pool);
        return NULL;
    }

    int created = 0;
    for (int i = 0; i < num_threads; i++) {
        if (pthread_create(&(pool->threads[i]), NULL, vibe_worker, (void*)pool) != 0) {
            // If creation fails, shutdown the pool to tell existing workers to exit
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

/**
 * vibe_thread_pool_add_job - Adds a new task to the thread pool queue.
 * @pool: Pointer to the thread pool
 * @function: The function to execute
 * @arg: The argument for the function
 *
 * Implements O(1) insertion at the tail of the queue.
 */
static inline void vibe_thread_pool_add_job(vibe_thread_pool_t* pool, void (*function)(void*), void* arg) {
    if (!pool || !function) return;

    vibe_job_t* job = (vibe_job_t*)malloc(sizeof(vibe_job_t));
    if (!job) return;

    job->function = function;
    job->arg = arg;
    job->next = NULL;

    pthread_mutex_lock(&(pool->lock));
    // BOLT: O(1) insertion using tail pointer, avoiding O(N) traversal inside lock
    if (pool->queue_tail == NULL) {
        pool->queue_head = job;
        pool->queue_tail = job;
    } else {
        pool->queue_tail->next = job;
        pool->queue_tail = job;
    }
    pool->queue_size++;
    pthread_cond_signal(&(pool->notify));
    pthread_mutex_unlock(&(pool->lock));
}

#endif
