/**
 * This header provides a portable wrapper for SIMD (Single Instruction, Multiple Data) intrinsics in the Vibe C library.
 * In version 1.5.11, it continues to automatically include the appropriate intrinsic headers based on detected architecture.
 * This code is AI-generated.
 */
#ifndef VIBE_SIMD_H
#define VIBE_SIMD_H

// Internal Logic: Detect the target architecture at compile-time and include the corresponding platform intrinsic headers.
#if defined(__x86_64__)
    #include <immintrin.h>
#elif defined(__aarch64__)
    #include <arm_neon.h>
#endif

#endif
