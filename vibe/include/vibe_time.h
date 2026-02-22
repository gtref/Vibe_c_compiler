/*
 * This header has been updated to include comprehensive documentation and logic explanations.
 * The changes enhance maintainability by detailing the high-resolution monotonic timing utilities for Vibe C.
 * THIS CODE IS AI GENERATED.
 */

#ifndef VIBE_TIME_H
#define VIBE_TIME_H
#include <time.h>
/**
 * vibe_get_time - Returns the current monotonic time in seconds.
 *
 * Uses CLOCK_MONOTONIC to ensure the time is not affected by OS clock adjustments.
 * Returns the time as a double-precision floating point number.
 */
static inline double vibe_get_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}
#endif
