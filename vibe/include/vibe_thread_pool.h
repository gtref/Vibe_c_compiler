#ifndef VIBE_THREAD_POOL_H
#define VIBE_THREAD_POOL_H

#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct vibe_job {
    void (*function)(void* arg);
    void* arg;
    struct vibe_job* next;
} vibe_job_t;

typedef struct {
    pthread_mutex_t lock;
    pthread_cond_t notify;
    pthread_t* threads;
    vibe_job_t* queue_head;
    vibe_job_t* queue_tail; // BOLT: Tail pointer for O(1) job insertion
    int thread_count;
    int queue_size;
    bool shutdown;
} vibe_thread_pool_t;

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

static inline vibe_thread_pool_t* vibe_thread_pool_create(int num_threads) {
    // Sentinel: Enforce a reasonable maximum thread count to prevent resource exhaustion and integer overflows
    if (num_threads <= 0 || num_threads > 1024) return NULL;

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

static inline void vibe_thread_pool_add_job(vibe_thread_pool_t* pool, void (*function)(void*), void* arg) {
    if (!pool || !function) return;

    vibe_job_t* job = (vibe_job_t*)malloc(sizeof(vibe_job_t));
    if (!job) return;

    job->function = function;
    job->arg = arg;
    job->next = NULL;

    pthread_mutex_lock(&(pool->lock));

    // Sentinel: Prevent adding jobs to a pool that is shutting down to avoid memory leaks and undefined behavior
    if (pool->shutdown) {
        pthread_mutex_unlock(&(pool->lock));
        free(job);
        return;
    }

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

/**
 * vibe_thread_pool_destroy - Safely shuts down the pool and frees resources
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
