/**
 * Simple POSIX threads (pthreads) wrapper for Vibe C.
 * Provides a unified and simplified interface for thread creation and joining.
 * This code is AI-generated.
 */
#ifndef VIBE_THREAD_H
#define VIBE_THREAD_H

#include <pthread.h>

/* Type definition for Vibe threads based on pthreads */
typedef pthread_t vibe_thread_t;

/* Macros for basic thread lifecycle management */
#define vibe_thread_create(t, f, a) pthread_create(t, NULL, f, a)
#define vibe_thread_join(t) pthread_join(t, NULL)

#endif
