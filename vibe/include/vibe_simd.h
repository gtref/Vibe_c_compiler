/**
 * Unified SIMD intrinsics wrapper for Vibe C.
 * Automatically includes the correct SIMD header (AVX/Neon) based on the target architecture.
 * This code is AI-generated.
 */
#ifndef VIBE_SIMD_H
#define VIBE_SIMD_H

/* Detect architecture and include appropriate SIMD intrinsics */
#if defined(__x86_64__)
    #include <immintrin.h>
#elif defined(__aarch64__)
    #include <arm_neon.h>
#endif

#endif
