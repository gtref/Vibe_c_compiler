/**
 * This header contains definitions and macros specifically for the x86_64 architecture.
 * In version 1.5.9, it defines VIBE_ARCH_X86_64 for x86_64 targets to enable optimized code paths and intrinsics.
 * This code is AI-generated.
 */
#ifndef VIBE_X86_64_H
#define VIBE_X86_64_H

// Internal Logic: Detect if the current compilation target is x86_64 using standard compiler macros.
#ifdef __x86_64__
// Internal Logic: Define the architecture macro to enable x86-specific optimizations across the library.
#define VIBE_ARCH_X86_64
#endif

#endif
