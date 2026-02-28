/**
 * This header provides a high-performance worker thread pool implementation for the Vibe C library.
 * In version 1.5.8, it features O(1) job insertion and hardened initialization to prevent data races.
 * This code is AI-generated.
 */
#ifndef VIBE_THREAD_POOL_H
#define VIBE_THREAD_POOL_H

#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>

// Internal Logic: Structure representing a single unit of work in the queue.
typedef struct vibe_job {
    void (*function)(void* arg);
    void* arg;
    struct vibe_job* next;
} vibe_job_t;

// Internal Logic: The main thread pool structure managing workers and the job queue.
typedef struct {
    pthread_mutex_t lock;
    pthread_cond_t notify;
    pthread_t* threads;
    vibe_job_t* queue_head;
    vibe_job_t* queue_tail; // BOLT: Tail pointer for O(1) job insertion
    int thread_count;
    int queue_size;
    int max_queue_size;
    bool shutdown;
} vibe_thread_pool_t;

/**
 * vibe_worker - Internal worker thread routine.
 * Internal Logic: Waits for jobs on a condition variable and executes them until shutdown is signaled.
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
 * vibe_thread_pool_create - Initializes a new thread pool.
 * Internal Logic: Validates thread count, allocates resources, and spawns worker threads.
 * Enforces a maximum thread limit to prevent resource exhaustion.
 */
static inline vibe_thread_pool_t* vibe_thread_pool_create(int num_threads) {
    if (num_threads <= 0 || num_threads > 1024) return NULL;
    vibe_thread_pool_t* pool = (vibe_thread_pool_t*)malloc(sizeof(vibe_thread_pool_t));
    if (!pool) return NULL;

    pool->thread_count = num_threads;
    pool->queue_size = 0;
    pool->max_queue_size = 65536; // Sentinel: Limit queue size to prevent DoS.
    pool->queue_head = NULL;
    pool->queue_tail = NULL;
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
            // Internal Logic: If thread creation fails, we must signal shutdown while holding the lock.
            // This prevents a race condition where a worker might miss the shutdown signal.
            pthread_mutex_lock(&(pool->lock));
            pool->shutdown = true;
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
 * vibe_thread_pool_add_job - Adds a new job to the pool's queue.
 * Internal Logic: Uses a tail pointer for O(1) insertion and signals a worker.
 * Rejects jobs if the pool is full or shutting down.
 */
static inline void vibe_thread_pool_add_job(vibe_thread_pool_t* pool, void (*function)(void*), void* arg) {
    if (!pool || !function) return;
    vibe_job_t* job = (vibe_job_t*)malloc(sizeof(vibe_job_t));
    if (!job) return;
    job->function = function;
    job->arg = arg;
    job->next = NULL;
    pthread_mutex_lock(&(pool->lock));
    if (pool->shutdown || pool->queue_size >= pool->max_queue_size) {
        pthread_mutex_unlock(&(pool->lock));
        free(job);
        return;
    }
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

/**
 * vibe_thread_pool_destroy - Safely shuts down the pool and frees all resources.
 * Internal Logic: Signals shutdown, joins all threads, and drains the remaining job queue.
 */
static inline void vibe_thread_pool_destroy(vibe_thread_pool_t* pool) {
    if (!pool) return;
    pthread_mutex_lock(&(pool->lock));
    pool->shutdown = true;
    pthread_cond_broadcast(&(pool->notify));
    pthread_mutex_unlock(&(pool->lock));
    for (int i = 0; i < pool->thread_count; i++) {
        pthread_join(pool->threads[i], NULL);
    }
    vibe_job_t* curr = pool->queue_head;
    while (curr) {
        vibe_job_t* next = curr->next;
        free(curr);
        curr = next;
    }
    free(pool->threads);
    pthread_mutex_destroy(&(pool->lock));
    pthread_cond_destroy(&(pool->notify));
    free(pool);
}

#endif
