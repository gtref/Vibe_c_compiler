# Vibe C Compiler - Architecture

## Overview

The Vibe C Compiler is a high-level project manager and compiler driver for C. It is designed to simplify the development workflow by providing a structured project environment, high-performance builds, and built-in security auditing.

## Core Components

### 1. The Compiler Driver (`VibeCompiler`)
Located in `vibe/core/compiler.py`, this class is the heart of the system. It encapsulates all project management and compilation logic.

- **Project Lifecycle**: Methods like `init_project`, `build_project`, `run_project`, and `clean_project` manage the standard development cycle.
- **Incremental Build Engine**: Uses file modification times (`os.path.getmtime`) and cached header metadata to skip redundant compilation steps.
- **Parallel Task Runner**: Leverages `ThreadPoolExecutor` for concurrent compilation, test execution, and security auditing.
- **Security Auditor**: Implements a high-performance, parallelized regex-based scanning system for vulnerabilities.

### 2. Built-in Headers (`vibe/include/`)
Vibe C provides a suite of 25 custom headers that offer a "standard library" experience for common tasks like SIMD, JSON parsing, networking, and UI.

### 3. Template System (`vibe/templates/`)
Supports quick project initialization using pre-defined directory structures (e.g., `basic`, `minimal`).

### 4. CLI Interface (`main.py` & `vibe_c_compiler`)
A clean command-line interface built with `argparse`, and an optional interactive TUI menu (`menu.py`).

## Compilation Flow

1.  **Project Discovery**: The compiler reads `vibe.json` to understand the project configuration (name, type, etc.).
2.  **Header Scanning**: Performs an efficient stack-based scan of `src/` and the global `vibe/include/` directory to find the latest modification time among all `.h` files.
3.  **Source Filtering**: Identifies `.c` files in `src/` that are newer than their corresponding `.o` files in `build/obj/` or newer than the latest header change.
4.  **Parallel Compilation**: Spawns worker threads to run `clang` on each source file that needs compilation.
5.  **Linking**: Combines object files into the final executable, static library, or shared library.

## Security Architecture (Sentinel 🛡️)

- **Defense in Depth**: Combines input validation, safe subprocess management, and static analysis.
- **Input Sanitization**: All CLI arguments and `vibe.json` values are validated against strict regex patterns before being used in file paths or commands.
- **Sanitized Environments**: Ensures environment variables like `LD_LIBRARY_PATH` do not contain empty entries that could lead to library hijacking.
- **Binary Hardening**: Automatically applies industry-standard security flags (`-fstack-protector-strong`, PIE, RELRO, etc.) to all compiled binaries and tests to mitigate memory corruption exploits (v1.4.5).
- **Audit System Optimization**: Uses `re.finditer` and $O(\log N)$ line-numbering for rapid scanning, with word-boundary matching and `nosec` suppression support (v1.4.7-v1.5.3). As of v1.4.8, it also performs self-auditing of internal headers and includes expanded pattern detection.
- **Secure Memory Primitives**: Implemented `vibe_secure_memzero` in `vibe_mem.h` to reliably clear sensitive data using volatile pointers (v1.4.8).
- **String Security**: Implemented `vibe_str_eq_constant_time` to mitigate timing attacks on sensitive string comparisons with a single-pass implementation (v1.4.9-v1.5.6).
- **Network Hardening**: Implemented zero-initialization of network structures and `SOMAXCONN` listen backlogs to prevent information leakage and DoS attacks (v1.5.0).
- **Regex Safety**: `vibe_regex.h` includes NULL pointer checks to prevent crashes when matching unvalidated inputs (v1.5.2).
- **File I/O Hardening**: `vibe_file.h` prevents OOM/DoS by enforcing a 10MB file size limit and verifying all file positioning system calls (v1.5.10).
- **String and I/O Hardening**: Implemented `vibe_str_copy` for guaranteed null-termination and `vibe_snprintf`/`vibe_vsnprintf` for format string literal enforcement (v1.5.11).
- **Thread Pool Robustness**: `vibe_thread_pool.h` implements robust error handling for `malloc`, `pthread_mutex_init`, `pthread_cond_init`, and `pthread_create`, with atomic cleanup logic to prevent resource leaks and crashes. It features a hard 1024 thread limit (v1.5.1-v1.5.11).
- **Compile-time Format String Hardening**: Hardens variadic macros in `vibe_io.h` and `vibe_log.h` by using string literal concatenation to prefix the format string with a literal, eliminating format string injection at the source (v1.5.1).
- **Library Robustness**: Core headers include NULL pointer checks and secure, fully-compliant JSON escaping with recursive depth tracking (v1.4.7-v1.5.11).

## Performance Engineering (Bolt ⚡)

- **Minimizing I/O**: Efficient stack-based `os.scandir` traversal and metadata caching for global headers.
- **Chunked I/O Processing**: String output functions (like `vibe_json_print`) implement a chunked I/O pattern, grouping non-special characters into a single `fwrite` call using `strcspn`. This significantly reduces the number of system calls compared to character-by-character output (v1.4.7).
- **Arithmetic Optimization**: The `vibe_xor_cipher` function optimizes throughput by replacing the modulo operator with an incremental index and specializing paths for 1, 2, 4, 8, and 16-byte keys using SWAR (v1.5.2-v1.5.7).
- **O(1) Concurrency**: The `vibe_thread_pool.h` implementation uses both head and tail pointers for the job queue, ensuring constant-time insertion even as the queue size grows, reducing lock contention (v1.5.1).
- **Optimized Regex**: Use of combined regular expressions for $O(1)$ pattern matching per line in the security auditor.
- **Concurrency**: Maximum utilization of CPU cores for CPU-bound compilation tasks.

---

## Project Navigation

- [Home (README)](../../README.md)
- [Documentation](../README.md)
- [Developer Docs](README.md)
- [Security Policy](../../SECURITY.md)
- [Contributing Guidelines](../../CONTRIBUTING.md)
- [Contributors](../../CONTRIBUTORS.md)
- [Code of Conduct](../../CODE_OF_CONDUCT.md)
