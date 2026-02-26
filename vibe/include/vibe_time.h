/**
 * Time utility functions for Vibe C.
 * Provides a high-resolution monotonic timer for performance measurement and scheduling.
 * This code is AI-generated.
 */
#ifndef VIBE_TIME_H
#define VIBE_TIME_H

#include <time.h>

/* Returns the current monotonic time in seconds with nanosecond precision */
static inline double vibe_get_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}
#endif
