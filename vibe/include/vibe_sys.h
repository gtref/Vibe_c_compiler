/**
 * Operating system detection macros for Vibe C.
 * Provides unified macros for Windows, Linux, and macOS platforms.
 * This code is AI-generated.
 */
#ifndef VIBE_SYS_H
#define VIBE_SYS_H

/* Unified OS detection based on compiler-defined macros */
#if defined(_WIN32)
    #define VIBE_OS_WINDOWS
#elif defined(__linux__)
    #define VIBE_OS_LINUX
#elif defined(__APPLE__)
    #define VIBE_OS_MACOS
#endif

#endif
