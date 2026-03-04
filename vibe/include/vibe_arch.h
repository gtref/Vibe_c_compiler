/**
 * This header serves as a central entry point for architecture-specific definitions in the Vibe C library.
 * In version 1.5.11, it provides a unified interface for x86_64, ARM64, and RISC-V targets by aggregating headers.
 * This code is AI-generated.
 */
#ifndef VIBE_ARCH_H
#define VIBE_ARCH_H

// Internal Logic: Aggregate all architecture-specific headers to provide a single point of inclusion.
#include "vibe_x86_64.h"
#include "vibe_arm64.h"
#include "vibe_rv64.h"

#endif
