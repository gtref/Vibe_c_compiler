# Vibe C Compiler - Developer Documentation

Welcome to the internal documentation for Vibe C Compiler.

## Architecture Overview

The Vibe C Compiler is a Python-based driver that wraps Clang to provide a more streamlined C development experience. For a deep dive into the system components, see [Architecture](architecture.md).

### Directory Structure

- `vibe/`: Core package containing all compiler logic.
  - `core/`: Main Python logic.
    - `main.py`: Entry point and CLI argument parsing.
    - `compiler.py`: The `VibeCompiler` class, implementing all commands and build logic.
    - `menu.py`: TUI menu implementation.
  - `include/`: Custom C headers provided by Vibe C.
  - `templates/`: Project scaffolding templates (e.g., `basic`, `minimal`).
- `vibe_c_compiler`: Root executable script.
- `Jules/`: Internal developer logs and security records.
- `DOCS/`: Project documentation.

## Build System (Bolt ⚡)

The build system in `vibe/core/compiler.py` is designed for speed (Bolt philosophy).

### Parallel Execution
We use `concurrent.futures.ThreadPoolExecutor` to parallelize performance-critical tasks.
- **Builds**: Compiles multiple `.c` files in parallel.
- **Tests**: Compiles and runs multiple test files concurrently, significantly reducing the test cycle time.
- **Security Audit**: Scans project files across multiple threads for rapid vulnerability detection (v1.4.6).

### I/O Optimizations (Chunked I/O)
To maximize I/O throughput, string processing functions (like `vibe_json_print`) use a chunked I/O pattern. Instead of printing character-by-character, "normal" characters are accumulated and printed in bulk using `fwrite`, significantly reducing system call overhead (v1.4.7).

### Concurrency Optimizations
- **O(1) Job Insertion**: The worker thread pool in `vibe_thread_pool.h` maintains both head and tail pointers for its job queue. This ensures $O(1)$ job insertion and minimizes lock contention even with large numbers of pending tasks (v1.5.1).
- **SWAR Hashing**: The `vibe_simple_hash` function in `vibe_crypt.h` is optimized using SWAR (SIMD Within A Register) to process 8 bytes at a time, utilizing word-sized loads and bitmask-based null terminator detection for high-speed hashing (v1.5.9).

### Arithmetic Optimizations
- **Modulo Elimination**: The `vibe_xor_cipher` function in `vibe_crypt.h` replaces the modulo operator (`%`) with an incremental index and conditional reset. This avoids expensive division instructions in the hot loop (v1.5.2).
- **SWAR Specialization**: Specialized paths for 1, 2, 4, 8, and 16-byte keys using 64-bit word-sized operations (SWAR) yield massive performance gains for the XOR cipher (v1.5.7).

### Incremental Logic
- **Centralized Scanning**: A reusable `_get_header_mtime` method performs a single-pass scan of `src/` and `vibe/include/` for headers using efficient stack-based `os.scandir`.
- **Non-Recursive Traversals**: All directory scanning and file collection logic in `vibe/core/compiler.py` (build, test, audit) uses non-recursive, stack-based traversals with `os.scandir` to improve performance and prevent recursion depth issues (v1.5.9).
- **Modification Times**: We compare the `mtime` of source files and headers against existing artifacts.
- **Build Incrementalism**: Checks `.c` and `.h` files against object files in `build/obj/`.
- **Test Incrementalism**: Checks test source files, project headers, and the compiled project library against test binaries in `build/tests/`.
- **NO-OP Optimization**: Before starting the thread pool, we filter out files that are already up-to-date. This avoids the overhead of managing a thread pool when nothing needs to be done (v1.4.1/v1.4.2).

## Security (Sentinel 🛡️)

Security is a core pillar of Vibe C (Sentinel philosophy).

### Input Validation
Every user-provided argument that affects file paths or shell commands is strictly validated using regex:
- Project names and templates: `^[a-zA-Z0-9_-]+$`
- Architecture targets: `^[a-zA-Z0-9._-]+$`

### Safe Execution
- We use `subprocess.run` with argument lists (not shell strings) to prevent command injection.
- Absolute paths are used where appropriate.
- **Command Anchoring**: The `update` command is pinned to the compiler's root directory (`self.base_dir`) in `subprocess.run(..., cwd=self.base_dir)` to prevent accidental modification of user projects (v1.4.4).

### Internal Audit
The `audit` command uses an optimized regex-based detection system to identify common C vulnerabilities and Python security anti-patterns.
- **Combined Regex**: Patterns are combined into a single pre-compiled regex with alternation to ensure $O(Lines)$ complexity, avoiding the $O(Lines \times Patterns)$ overhead of multiple passes (v1.4.4).
- **Efficient Line Mapping**: Uses `re.finditer` on the entire file content combined with a `bisect`-based $O(\log N)$ line-numbering algorithm for rapid location of issues (v1.4.6).
- **Named Groups**: Python pattern matching uses named capture groups for efficient identification of the specific vulnerability detected.
- **Word Boundaries**: C and Python patterns use `\b` word boundaries to prevent false positives and bypasses like `(printf)("...")` (v1.4.7).
- **Self-Auditing**: As of v1.4.8, the audit tool also scans the compiler's internal headers (`vibe/include/`) and includes an expanded set of 8 additional C functions and 3 Python patterns.
- **Suppression**: Added support for `// nosec` and `/* nosec */` in C/Header files to reduce false positives (v1.5.3).

### Environment Sanitization
- **LD_LIBRARY_PATH**: In `run_tests`, we explicitly sanitize `LD_LIBRARY_PATH` by splitting it, filtering out empty entries (which are interpreted as the current directory `.` by the dynamic linker), and then prepending the `build` directory. This mitigates shared library injection vulnerabilities (v1.4.4).

### Secure Utilities
- **JSON Printing**: The `vibe_json.h` header includes a secure string printing helper that escapes double quotes, backslashes, and all control characters (U+0000 to U+001F). It also features recursive depth tracking (max 128) to mitigate stack overflow DoS (v1.4.7-v1.5.8).
- **Secure Memory**: `vibe_mem.h` provides `vibe_secure_memzero`, which uses a `volatile` pointer and word-sized writes to ensure that memory is actually cleared and not optimized away by the compiler (v1.4.8).
- **String Security**: `vibe_string.h` implements `vibe_str_eq_constant_time` with a hardened single-pass implementation to mitigate timing attacks on sensitive string comparisons (v1.4.9-v1.5.6).
- **Network Hardening**: `vibe_net.h` implements zero-initialization of `sockaddr_in` structures, uses `SOMAXCONN` for listening backlogs, and includes port range validation, `SO_REUSEADDR`, and `FD_CLOEXEC` for improved robustness and resource isolation (v1.5.0-v1.5.9).
- **Regex Safety**: `vibe_regex.h` includes NULL pointer checks for pattern and text arguments to prevent crashes (v1.5.2).
- **Library Robustness**: Core library functions in `vibe_json.h`, `vibe_crypt.h`, `vibe_file.h`, `vibe_string.h`, `vibe_net.h`, `vibe_regex.h`, and `vibe_thread_pool.h` include NULL pointer checks on their inputs to prevent runtime crashes (v1.4.7-v1.5.8).
- **Thread Pool Robustness**: `vibe_thread_pool.h` implements robust error handling for `malloc`, `pthread_mutex_init`, `pthread_cond_init`, and `pthread_create`, ensuring that any initialization failure results in an atomic cleanup of resources. It now also resolves a critical race condition in initialization error paths (v1.5.1-v1.5.8).
- **Compile-time Format String Hardening**: Printing macros in `vibe_io.h` and `vibe_log.h` use string literal concatenation to prefix the format string with a literal, preventing format string injection vulnerabilities at compile-time (v1.5.1).

### Binary Hardening
Vibe C automatically applies security hardening flags during the compilation and linking phases to protect produced binaries against common exploits (v1.4.5):
- **Stack Protection**: Uses `-fstack-protector-strong` to protect against stack buffer overflows.
- **Fortify Source**: Enables `-D_FORTIFY_SOURCE=2` for additional run-time checks on standard library functions.
- **Format String Security**: Enforces `-Wformat`, `-Wformat-security`, and `-Werror=format-security` to prevent format string vulnerabilities.
- **ASLR (PIE)**: Compiles with `-fPIE` and links with `-pie` to ensure Position Independent Executables, enabling Address Space Layout Randomization.
- **Linker Hardening**: Uses `-Wl,-z,relro,-z,now` for full Relocation Read-Only (RELRO) and immediate binding to protect the Global Offset Table (GOT).

## Contributing

When making changes:
1.  **Keep it fast**: Ensure that build times remain low.
2.  **Keep it secure**: Always validate new inputs.
3.  **Document everything**: Update the relevant `MD` files and the SENTINEL security logs.

---

## Project Navigation

- [Home (README)](../../README.md)
- [Documentation](../README.md)
- [Developer Docs](README.md)
- [Security Policy](../../SECURITY.md)
- [Contributing Guidelines](../../CONTRIBUTING.md)
- [Contributors](../../CONTRIBUTORS.md)
- [Code of Conduct](../../CODE_OF_CONDUCT.md)
