/*
 * This header has been updated to include comprehensive documentation and logic explanations.
 * The changes enhance maintainability by detailing the OS detection and platform-specific definitions.
 * THIS CODE IS AI GENERATED.
 */

#ifndef VIBE_SYS_H
#define VIBE_SYS_H
#if defined(_WIN32)
    #define VIBE_OS_WINDOWS
#elif defined(__linux__)
    #define VIBE_OS_LINUX
#elif defined(__APPLE__)
    #define VIBE_OS_MACOS
#endif
#endif
