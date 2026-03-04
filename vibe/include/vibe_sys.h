/**
 * This header provides platform detection macros for the Vibe C library.
 * In version 1.5.11, it correctly identifies Windows, Linux, and macOS platforms at compile-time for cross-platform support.
 * This code is AI-generated.
 */
#ifndef VIBE_SYS_H
#define VIBE_SYS_H

// Internal Logic: Use compiler-predefined macros to detect the underlying platform during compilation.
#if defined(_WIN32)
    #define VIBE_OS_WINDOWS
#elif defined(__linux__)
    #define VIBE_OS_LINUX
#elif defined(__APPLE__)
    #define VIBE_OS_MACOS
#endif

#endif
