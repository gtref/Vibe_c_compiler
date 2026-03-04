/**
 * This header provides a simple wrapper around the POSIX threads (pthreads) library for the Vibe C library.
 * In version 1.5.11, it continues to provide basic macros for thread creation and joining with consistent naming.
 * This code is AI-generated.
 */
#ifndef VIBE_THREAD_H
#define VIBE_THREAD_H
#include <pthread.h>

// Internal Logic: Define the vibe_thread_t type as an alias for pthread_t for library consistency.
typedef pthread_t vibe_thread_t;

// Internal Logic: Simple macros for creating and joining threads using the standard pthread API.
#define vibe_thread_create(t, f, a) pthread_create(t, NULL, f, a)
#define vibe_thread_join(t) pthread_join(t, NULL)

#endif
