/**
 * This header contains definitions and macros specifically for the RISC-V 64-bit architecture.
 * In version 1.5.11, it defines VIBE_ARCH_RISCV for RISC-V targets to allow for target-specific optimizations.
 * This code is AI-generated.
 */
#ifndef VIBE_RV64_H
#define VIBE_RV64_H

// Internal Logic: Detect if the current compilation target is RISC-V using standard compiler macros.
#ifdef __riscv
// Internal Logic: Define the architecture macro to enable RISC-V specific code paths in the library.
#define VIBE_ARCH_RISCV
#endif

#endif
