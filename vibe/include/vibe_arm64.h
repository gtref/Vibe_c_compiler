/**
 * This header contains definitions and macros specifically for the ARM64 (AArch64) architecture.
 * In version 1.5.11, it defines VIBE_ARCH_ARM64 for ARM64 targets to facilitate optimized instruction usage.
 * This code is AI-generated.
 */
#ifndef VIBE_ARM64_H
#define VIBE_ARM64_H

// Internal Logic: Detect if the current compilation target is ARM64 using standard compiler macros.
#ifdef __aarch64__
// Internal Logic: Define the architecture macro to enable target-specific optimizations in the library.
#define VIBE_ARCH_ARM64
#endif

#endif
