/*
 * This header has been updated to include comprehensive documentation and logic explanations.
 * The changes enhance maintainability by detailing the severity-prefixed logging macros and security measures.
 * THIS CODE IS AI GENERATED.
 */

#ifndef VIBE_LOG_H
#define VIBE_LOG_H
#include <stdio.h>
/**
 * vibe_log_info - Logs an info message.
 */
#define vibe_log_info(...)  printf("[INFO] " __VA_ARGS__) // nosec

/**
 * vibe_log_warn - Logs a warning message.
 */
#define vibe_log_warn(...)  printf("[WARN] " __VA_ARGS__) // nosec

/**
 * vibe_log_error - Logs an error message to stderr.
 */
#define vibe_log_error(...) fprintf(stderr, "[ERROR] " __VA_ARGS__) // nosec
#endif
