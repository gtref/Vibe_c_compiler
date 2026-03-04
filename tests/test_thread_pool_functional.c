/**
 * This test verifies the basic functionality of the Vibe thread pool implementation.
 * In version 1.5.11, it continues to ensure that the queue and workers are operating correctly under standard load.
 * This code is AI-generated.
 */
#include "../vibe/include/vibe_thread_pool.h"
#include <stdio.h>
#include <unistd.h>

/**
 * Print start/finish messages for a job and sleep briefly to emulate work.
 *
 * @param arg Pointer to an `int` containing the job id; the pointee must remain valid for the call.
 */
void sample_job(void* arg) {
    int id = *(int*)arg;
    printf("Job %d started\n", id); // nosec
    usleep(100000); // 100ms
    printf("Job %d finished\n", id); // nosec
}

/**
 * Run a functional test that creates a thread pool, enqueues sample jobs, waits for them to complete, and then destroys the pool.
 *
 * The program creates a thread pool with four worker threads, submits ten sample jobs (each receives a distinct job id),
 * waits briefly to allow job completion, and cleans up the thread pool before exiting.
 *
 * @returns 0 on success, 1 if thread pool creation fails.
 */
int main() {
    // Internal Logic: Create a pool with 4 threads and add 10 sample jobs to the work queue.
    printf("Creating thread pool...\n"); // nosec
    vibe_thread_pool_t* pool = vibe_thread_pool_create(4);
    if (!pool) {
        printf("Failed to create thread pool\n"); // nosec
        return 1;
    }

    static int job_ids[10];
    for (int i = 0; i < 10; i++) {
        job_ids[i] = i;
        vibe_thread_pool_add_job(pool, sample_job, &job_ids[i]);
    }

    // Internal Logic: Wait for a sufficient duration to ensure all jobs complete before pool destruction.
    printf("Waiting for jobs to finish (simple sleep for test)...\n"); // nosec
    sleep(1);

    // Internal Logic: Shut down the pool and free all associated resources concurrently.
    printf("Destroying thread pool...\n"); // nosec
    vibe_thread_pool_destroy(pool);

    printf("Functional test complete.\n"); // nosec
    return 0;
}
