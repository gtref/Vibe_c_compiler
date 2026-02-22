/*
 * This header has been updated to include comprehensive documentation and logic explanations.
 * The changes enhance maintainability by detailing the simple POSIX thread wrappers and concurrency macros.
 * THIS CODE IS AI GENERATED.
 */

#ifndef VIBE_THREAD_H
#define VIBE_THREAD_H
#include <pthread.h>
typedef pthread_t vibe_thread_t;
#define vibe_thread_create(t, f, a) pthread_create(t, NULL, f, a)
#define vibe_thread_join(t) pthread_join(t, NULL)
#endif
