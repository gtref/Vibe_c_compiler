/**
 * This header provides high-resolution timing utilities for the Vibe C library.
 * In version 1.5.11, it implements vibe_get_time using the monotonic clock for accurate, drift-free duration measurement.
 * This code is AI-generated.
 */
#ifndef VIBE_TIME_H
#define VIBE_TIME_H
#include <time.h>

/**
 * vibe_get_time - Returns the current monotonic time in seconds.
 * Internal Logic: Uses clock_gettime with CLOCK_MONOTONIC for high-resolution, non-drifting time measurement.
 */
static inline double vibe_get_time() {
    // Internal Logic: Capture the current value of the monotonic clock to ensure time doesn't jump backwards.
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}

#endif
