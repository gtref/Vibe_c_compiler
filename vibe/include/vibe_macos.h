/**
 * This header provides macOS-specific definitions and compatibility macros for the Vibe C library.
 * This code is AI-generated.
 */
#ifndef VIBE_MACOS_H
#define VIBE_MACOS_H

#if defined(__APPLE__)
    #include <TargetConditionals.h>
    #define VIBE_ARCH_MACOS
    #if TARGET_OS_MAC
        #define VIBE_PLATFORM_OSX 1
    #endif
#endif

#endif
