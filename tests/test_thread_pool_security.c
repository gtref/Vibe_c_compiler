/**
 * This test verifies the security hardening of the thread pool implementation.
 * In version 1.5.11, it checks the maximum thread limit, job rejection during shutdown, and queue size enforcement.
 * This code is AI-generated.
 */
#include "../vibe/include/vibe_thread_pool.h"
#include "../vibe/include/vibe_test.h"
#include <unistd.h>

/**
 * empty_job - A no-op job for testing queue behavior.
 */
void empty_job(void* arg) {
    (void)arg;
}

int main() {
    printf("Starting thread pool security tests...\n"); // nosec

    // Internal Logic: Verify that attempting to create a pool exceeding the 1024 thread limit is rejected.
    vibe_thread_pool_t* oversized_pool = vibe_thread_pool_create(1025);
    VIBE_ASSERT(oversized_pool == NULL);
    if (oversized_pool) vibe_thread_pool_destroy(oversized_pool);

    // Internal Logic: Ensure a standard sized pool is successfully initialized.
    vibe_thread_pool_t* pool = vibe_thread_pool_create(4);
    VIBE_ASSERT(pool != NULL);

    // Internal Logic: Manually trigger the shutdown flag to verify that new jobs are correctly rejected.
    pthread_mutex_lock(&(pool->lock));
    pool->shutdown = true;
    pthread_mutex_unlock(&(pool->lock));

    vibe_thread_pool_add_job(pool, empty_job, NULL);
    VIBE_ASSERT(pool->queue_size == 0); // Job should have been rejected and freed

    // Internal Logic: Reset shutdown and set a small queue limit to verify OOM protection behavior.
    pthread_mutex_lock(&(pool->lock));
    pool->shutdown = false;
    pool->max_queue_size = 2;
    pthread_mutex_unlock(&(pool->lock));

    vibe_thread_pool_add_job(pool, empty_job, NULL);
    vibe_thread_pool_add_job(pool, empty_job, NULL);
    VIBE_ASSERT(pool->queue_size == 2);

    // Internal Logic: Verify that additional jobs are rejected once the max queue size is reached.
    vibe_thread_pool_add_job(pool, empty_job, NULL);
    VIBE_ASSERT(pool->queue_size == 2);

    // Internal Logic: Signal workers and safely destroy the pool to clean up resources.
    pthread_mutex_lock(&(pool->lock));
    pthread_cond_broadcast(&(pool->notify));
    pthread_mutex_unlock(&(pool->lock));

    vibe_thread_pool_destroy(pool);

    VIBE_TEST_SUMMARY();
}
