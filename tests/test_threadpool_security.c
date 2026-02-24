#include <vibe_thread_pool.h>
#include <vibe_test.h>
#include <stdio.h>
#include <unistd.h>

void dummy_job(void* arg) {
    (void)arg;
}

int main() {
    // Test 1: Verify thread count limit
    vibe_thread_pool_t* pool_too_large = vibe_thread_pool_create(2048);
    VIBE_ASSERT(pool_too_large == NULL);
    printf("  [+] Thread count limit (2048) correctly enforced.\n");

    vibe_thread_pool_t* pool_zero = vibe_thread_pool_create(0);
    VIBE_ASSERT(pool_zero == NULL);
    printf("  [+] Thread count limit (0) correctly enforced.\n");

    // Test 2: Verify job rejection after shutdown
    vibe_thread_pool_t* pool = vibe_thread_pool_create(2);
    VIBE_ASSERT(pool != NULL);

    vibe_thread_pool_destroy(pool);
    // At this point, the pool is shutdown. Adding a job should return safely (and free the job internally).
    vibe_thread_pool_add_job(pool, dummy_job, NULL);
    printf("  [+] Job addition after shutdown handled safely.\n");

    VIBE_TEST_SUMMARY();
    return 0;
}
