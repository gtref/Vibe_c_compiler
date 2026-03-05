# Security Audit

Vibe C includes a built-in security audit command to help identify potential vulnerabilities:

```bash
vcc audit
```

## Security Architecture (Sentinel 🛡️)

- **Defense in Depth**: Combines input validation, safe subprocess management, and static analysis.
- **Input Sanitization**: All CLI arguments and `vibe.json` values are validated against strict regex patterns before being used in file paths or commands.
- **Sanitized Environments**: Ensures environment variables like `LD_LIBRARY_PATH` do not contain empty entries that could lead to library hijacking.
- **Binary Hardening**: Automatically applies industry-standard security flags (`-fstack-protector-strong`, PIE, RELRO, etc.) to all compiled binaries and tests.
- **Audit System Optimization**: Uses a high-performance, parallelized regex-based scanning system for rapid vulnerability scanning.
- **Secure Memory Primitives**: Implemented `vibe_secure_memzero` in `vibe_mem.h` to reliably clear sensitive data.
- **String Security**: Implemented `vibe_str_eq_constant_time` to mitigate timing attacks on sensitive string comparisons.
- **Network Hardening**: Implemented zero-initialization of network structures and `SOMAXCONN` listen backlogs.
- **File I/O Hardening**: `vibe_file.h` prevents OOM/DoS by enforcing a 10MB file size limit and verifying all file positioning system calls.
- **String and I/O Hardening**: Implemented `vibe_str_copy` for guaranteed null-termination and `vibe_snprintf`/`vibe_vsnprintf` for format string literal enforcement.
- **Thread Pool Robustness**: `vibe_thread_pool.h` implements robust error handling and features a hard 1024 thread limit.

## Audit Tools

1.  **Internal Audit**: Built-in pattern-based checks for common C and Python security issues. It scans `src/`, `vibe/`, and `tests/` for unsafe functions and Python anti-patterns.
2.  **Bandit** (Optional): A deeper security linter for Python (runs only if `bandit` is installed).
3.  **Cppcheck** (Optional): A more advanced static analysis tool for C/C++ (runs only if `cppcheck` is installed).
