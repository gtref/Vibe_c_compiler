/*
 * This header has been updated to include comprehensive documentation and logic explanations.
 * The changes enhance maintainability by detailing the unit testing framework and assertion macros.
 * THIS CODE IS AI GENERATED.
 */

#ifndef VIBE_TEST_H
#define VIBE_TEST_H

#include <stdio.h>
#include <string.h>

static int vibe_tests_run = 0;    /* Counter for total tests executed */
static int vibe_tests_failed = 0; /* Counter for total tests failed */

/**
 * VIBE_ASSERT - Basic assertion macro.
 * @cond: The condition to evaluate
 *
 * Increments the run counter. If the condition is false, increments the
 * failure counter and prints a failure message with file and line info.
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

/**
 * VIBE_ASSERT_EQ - Asserts that two values are equal.
 */
#define VIBE_ASSERT_EQ(a, b) VIBE_ASSERT((a) == (b))

/**
 * VIBE_ASSERT_STR_EQ - Asserts that two strings are equal.
 */
#define VIBE_ASSERT_STR_EQ(a, b) VIBE_ASSERT(strcmp((a), (b)) == 0)

/**
 * VIBE_TEST_SUMMARY - Prints a summary of all executed tests.
 *
 * Displays total, passed, and failed counts. Returns 1 if any tests failed.
 */
#define VIBE_TEST_SUMMARY() do { \
    printf("\n--- Test Summary ---\n"); /* nosec */ \
    printf("Total tests: %d\n", vibe_tests_run); /* nosec */ \
    printf("Passed:      %d\n", vibe_tests_run - vibe_tests_failed); /* nosec */ \
    printf("Failed:      %d\n", vibe_tests_failed); /* nosec */ \
    if (vibe_tests_failed > 0) return 1; \
} while(0)

#endif
