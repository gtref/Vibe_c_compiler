# Bolt's Journal ⚡

## Mission
Make the codebase faster, one optimization at a time. Speed is a feature. Every millisecond counts.

## Philosophy
- Measure first, optimize second.
- Don't sacrifice readability for micro-optimizations.
- Parallelism and caching are your best friends.

## 2024-02-07 - Parallel and Incremental Builds
**Learning:** Sequential compilation in Python via subprocess is significantly slower than parallel compilation, even for small files, due to underutilization of multi-core CPUs. A single Clang call with many files is also surprisingly slower than parallel individual calls in some environments.
**Action:** Implement a unified build system that compiles C files to object files in parallel and uses timestamps for incremental builds.
