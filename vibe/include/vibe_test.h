/**
 * Lightweight unit testing assertions for Vibe C.
 * Provides basic macros for condition checking, equality testing, and test summary generation.
 * This code is AI-generated.
 */
#ifndef VIBE_TEST_H
#define VIBE_TEST_H

#include <stdio.h>
#include <string.h>

/* Global counters for test execution and results */
static int vibe_tests_run = 0;
static int vibe_tests_failed = 0;

/**
 * Fundamental assertion macro that tracks success and failure
 */
#define VIBE_ASSERT(cond) do { \
    vibe_tests_run++; \
    if (!(cond)) { \
        printf("\033[31m[FAIL]\033[0m Assertion failed: %s at %s:%d\n", #cond, __FILE__, __LINE__); /* nosec */ \
        vibe_tests_failed++; \
    } else { \
        printf("\033[32m[PASS]\033[0m %s\n", #cond); /* nosec */ \
    } \
} while(0)

#define VIBE_ASSERT_EQ(a, b) VIBE_ASSERT((a) == (b))
#define VIBE_ASSERT_STR_EQ(a, b) VIBE_ASSERT(strcmp((a), (b)) == 0)

#define VIBE_TEST_SUMMARY() do { \
    printf("\n--- Test Summary ---\n"); /* nosec */ \
    printf("Total tests: %d\n", vibe_tests_run); /* nosec */ \
    printf("Passed:      %d\n", vibe_tests_run - vibe_tests_failed); /* nosec */ \
    printf("Failed:      %d\n", vibe_tests_failed); /* nosec */ \
    if (vibe_tests_failed > 0) return 1; \
} while(0)

#endif
