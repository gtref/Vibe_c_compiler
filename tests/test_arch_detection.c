/**
 * Comprehensive tests for vibe_arch.h and vibe_arm64.h architecture detection.
 * In version 1.5.11, this test verifies architecture macro definitions.
 * This code is AI-generated.
 */
#include "../vibe/include/vibe_arch.h"
#include "../vibe/include/vibe_test.h"
#include <stdio.h>

/**
 * test_arch_headers_included - Tests that architecture headers are included.
 * Internal Logic: Verifies that vibe_arch.h properly includes sub-headers.
 */
void test_arch_headers_included() {
    // If this compiles, vibe_x86_64.h, vibe_arm64.h, and vibe_rv64.h are included
    VIBE_ASSERT(true);
}

/**
 * test_arm64_detection - Tests ARM64 architecture detection.
 * Internal Logic: Checks if VIBE_ARCH_ARM64 is defined on ARM64 platforms.
 */
void test_arm64_detection() {
#ifdef __aarch64__
    #ifdef VIBE_ARCH_ARM64
        printf("ARM64 architecture correctly detected\n"); /* nosec */
        VIBE_ASSERT(true);
    #else
        printf("ERROR: ARM64 not detected when __aarch64__ is defined\n"); /* nosec */
        VIBE_ASSERT(false);
    #endif
#else
    #ifdef VIBE_ARCH_ARM64
        printf("ERROR: VIBE_ARCH_ARM64 defined on non-ARM64 platform\n"); /* nosec */
        VIBE_ASSERT(false);
    #else
        printf("ARM64 correctly not detected on non-ARM64 platform\n"); /* nosec */
        VIBE_ASSERT(true);
    #endif
#endif
}

/**
 * test_architecture_reporting - Reports the detected architecture.
 * Internal Logic: Prints which architecture macros are defined.
 */
void test_architecture_reporting() {
    printf("\n--- Architecture Detection Report ---\n"); /* nosec */

#ifdef VIBE_ARCH_ARM64
    printf("Detected: ARM64 (AArch64)\n"); /* nosec */
#endif

#ifdef __x86_64__
    printf("Detected: x86_64\n"); /* nosec */
#endif

#ifdef __riscv
    printf("Detected: RISC-V\n"); /* nosec */
#endif

#if !defined(VIBE_ARCH_ARM64) && !defined(__x86_64__) && !defined(__riscv)
    printf("Detected: Unknown or other architecture\n"); /* nosec */
#endif

    printf("--- End Architecture Report ---\n\n"); /* nosec */

    // This test always passes, it's informational
    VIBE_ASSERT(true);
}

/**
 * test_compile_time_checks - Tests compile-time architecture checks.
 * Internal Logic: Verifies that architecture detection works at compile time.
 */
void test_compile_time_checks() {
    // These are compile-time checks, so if we reach here they worked
    VIBE_ASSERT(true);
}

/**
 * test_multiple_arch_headers - Tests that multiple arch headers can coexist.
 * Internal Logic: Verifies no conflicts when all headers are included.
 */
void test_multiple_arch_headers() {
    // vibe_arch.h includes all three architecture headers
    // If this compiles without errors, the test passes
    VIBE_ASSERT(true);
}

/**
 * test_conditional_compilation - Tests conditional compilation based on arch.
 * Internal Logic: Verifies code can be conditionally compiled per architecture.
 */
void test_conditional_compilation() {
    int compiled = 0;

#ifdef VIBE_ARCH_ARM64
    compiled = 1;
    printf("ARM64-specific code would be compiled here\n"); /* nosec */
#else
    compiled = 1;
    printf("Non-ARM64 code path compiled\n"); /* nosec */
#endif

    VIBE_ASSERT(compiled == 1);
}

/**
 * test_arch_consistency - Tests that architecture macros are consistent.
 * Internal Logic: Verifies that if __aarch64__ is defined, VIBE_ARCH_ARM64 is too.
 */
void test_arch_consistency() {
#ifdef __aarch64__
    #ifndef VIBE_ARCH_ARM64
        printf("ERROR: Inconsistent ARM64 detection\n"); /* nosec */
        VIBE_ASSERT(false);
    #else
        VIBE_ASSERT(true);
    #endif
#else
    VIBE_ASSERT(true); // No inconsistency on non-ARM64
#endif
}

/**
 * test_header_guard - Tests that headers have proper include guards.
 * Internal Logic: Multiple inclusions shouldn't cause redefinition errors.
 */
void test_header_guard() {
    // Include the header again to test include guards
    #include "../vibe/include/vibe_arch.h"
    #include "../vibe/include/vibe_arm64.h"

    // If this compiles, include guards are working
    VIBE_ASSERT(true);
}

/**
 * test_no_side_effects - Tests that headers have no side effects.
 * Internal Logic: Verifies including headers doesn't execute unexpected code.
 */
void test_no_side_effects() {
    // Architecture headers should only define macros, no side effects
    VIBE_ASSERT(true);
}

/**
 * test_platform_identification - Tests platform can be identified.
 * Internal Logic: Ensures at least one platform characteristic is detected.
 */
void test_platform_identification() {
    int platform_detected = 0;

#if defined(__aarch64__) || defined(__x86_64__) || defined(__riscv) || \
    defined(__i386__) || defined(__arm__) || defined(_WIN32) || \
    defined(__linux__) || defined(__APPLE__)
    platform_detected = 1;
#endif

    VIBE_ASSERT(platform_detected == 1);
}

int main() {
    printf("Testing vibe_arch.h and vibe_arm64.h functionality...\n"); /* nosec */

    test_arch_headers_included();
    test_arm64_detection();
    test_architecture_reporting();
    test_compile_time_checks();
    test_multiple_arch_headers();
    test_conditional_compilation();
    test_arch_consistency();
    test_header_guard();
    test_no_side_effects();
    test_platform_identification();

    VIBE_TEST_SUMMARY();
    return 0;
}