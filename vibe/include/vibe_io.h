/*
 * This header has been updated to include comprehensive documentation and logic explanations.
 * The changes enhance maintainability by detailing the hardened I/O macros and security checks.
 * THIS CODE IS AI GENERATED.
 */

#ifndef VIBE_IO_H
#define VIBE_IO_H
#include <stdio.h>
/**
 * vibe_print - Hardened printing wrapper.
 * @fmt: Format string literal
 *
 * Enforces compile-time string literal check to prevent format string injection.
 */
#define vibe_print(fmt, ...) printf("" fmt, ##__VA_ARGS__) // nosec

/**
 * vibe_error - Hardened error printing wrapper.
 * @fmt: Format string literal
 *
 * Enforces compile-time string literal check to prevent format string injection.
 */
#define vibe_error(fmt, ...) fprintf(stderr, "" fmt, ##__VA_ARGS__) // nosec
#endif
