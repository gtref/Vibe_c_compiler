/**
 * This header provides a simple unit testing framework for the Vibe C library.
 * In version 1.5.9, it provides robust macros for assertions and colorized summary reporting for CLI integration.
 * This code is AI-generated.
 */
#ifndef VIBE_TEST_H
#define VIBE_TEST_H

#include <stdio.h>
#include <string.h>

// Internal Logic: Global counters for tracking test execution and failure within a translation unit.
static int vibe_tests_run = 0;
static int vibe_tests_failed = 0;

/**
 * VIBE_ASSERT - Checks a condition and prints the result.
 * Internal Logic: Increments global counters and uses ANSI codes for colorized pass/fail messages.
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

// Internal Logic: Helper macros for common equality assertions that use the core VIBE_ASSERT logic.
#define VIBE_ASSERT_EQ(a, b) VIBE_ASSERT((a) == (b))
#define VIBE_ASSERT_STR_EQ(a, b) VIBE_ASSERT(strcmp((a), (b)) == 0)

/**
 * VIBE_TEST_SUMMARY - Prints the final pass/fail statistics.
 * Internal Logic: Outputs a summary report and returns 1 if any tests failed, facilitating CLI integration.
 */
#define VIBE_TEST_SUMMARY() do { \
    printf("\n--- Test Summary ---\n"); /* nosec */ \
    printf("Total tests: %d\n", vibe_tests_run); /* nosec */ \
    printf("Passed:      %d\n", vibe_tests_run - vibe_tests_failed); /* nosec */ \
    printf("Failed:      %d\n", vibe_tests_failed); /* nosec */ \
    if (vibe_tests_failed > 0) return 1; \
} while(0)

#endif
