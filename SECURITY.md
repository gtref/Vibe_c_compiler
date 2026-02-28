# Security Policy

## Security Audit (v1.5.8)

A comprehensive security audit has been performed on the Vibe C Compiler v1.5.8 core and its custom headers.

### Core Compiler Logic
- **Input Validation**: All user-provided inputs (project names, templates, architectures) are now strictly validated against regex patterns across all commands (`init`, `build`, `run`, `test`, `status`, etc.) to mitigate path traversal and argument injection.
- **JSON Security**: The `vibe.json` configuration file is updated using the standard `json` library, preventing JSON injection vulnerabilities.
- **Subprocess Safety**: All calls to external tools (Clang, ar, etc.) use `subprocess.run` with argument lists and explicit working directories.
- **Safe Self-Updates**: The `vcc update` command explicitly executes within the compiler's installation directory, preventing unintended modifications to the user's workspace (v1.4.4).
- **Environment Safety**: In `vcc test`, the `LD_LIBRARY_PATH` is sanitized by removing empty entries to prevent shared library injection via the current directory (v1.4.4).
- **Optimized Security Audit**: The `vcc audit` command features an advanced parallelized regex-based detection system (v1.4.6). It uses pre-compiled combined patterns with word boundaries and named groups to identify unsafe C functions and Python patterns with high performance and accuracy. As of v1.4.8, it also performs self-auditing of the compiler's internal headers and detects an expanded set of 8 additional C functions and 3 Python patterns.

### Custom Headers
- **Robust File I/O**: `vibe_file.h` now includes checks for `ftell` failures and ensures that `fread` completes successfully, preventing issues with malformed files. Core functions now also include NULL pointer checks (v1.4.7).
- **Memory Management**: All custom headers now check for `malloc`/`strdup` failures.
- **Thread Pool Robustness**: `vibe_thread_pool.h` implements robust error handling for `malloc`, `pthread_mutex_init`, `pthread_cond_init`, and `pthread_create` with atomic cleanup logic to prevent resource leaks and crashes (v1.5.1).
- **Secure Memory**: `vibe_mem.h` now provides `vibe_secure_memzero` for reliably wiping sensitive data from memory (v1.4.8).
- **String Security**: `vibe_string.h` implements `vibe_str_eq_constant_time` to mitigate timing attacks on sensitive string comparisons (v1.4.9).
- **JSON Security**: `vibe_json.h` implements secure printing with full control character escaping (U+0000 to U+001F) to prevent injection and ensures robustness with NULL pointer checks (v1.4.7).
- **Network Security**: `vibe_net.h` implements zero-initialization of `sockaddr_in` structures to prevent stack memory leakage and uses `SOMAXCONN` as the default backlog to mitigate connection exhaustion DoS attacks (v1.5.0).
- **Regex Safety**: `vibe_regex.h` implements NULL pointer checks to prevent crashes when processing unvalidated inputs (v1.5.2).
- **Thread Pool Hardening**: `vibe_thread_pool.h` enforces a 1024 thread limit, includes shutdown-aware job rejection, and mitigates initialization-phase races (v1.5.8).
- **Library Robustness**: Core headers include NULL pointer checks, recursive depth tracking, and hardened I/O macros for increased stability and security (v1.4.7-v1.5.8).
- **Cryptography**: `vibe_crypt.h` provides a simple XOR cipher which is intended for obfuscation and educational purposes only. It is **not** suitable for securing sensitive data against determined attackers.

## Reporting a Vulnerability

If you find a security vulnerability in Vibe C Compiler, please do not open a public issue. Instead, follow these steps:
1. Contact the maintainers privately (see `CONTRIBUTORS.md` or repository meta).
2. Provide a detailed description of the vulnerability and a proof-of-concept if possible.

## Security Best Practices for Vibe C Projects
- Always check the return values of `vibe_alloc` and other memory-related functions.
- Be cautious when using `vibe_net.h` in production environments; ensure proper input validation on data received from the network.
- Use `vibe_regex.h` with trusted patterns to avoid potential ReDoS (Regular Expression Denial of Service).

---

## Project Navigation

- [Home (README)](README.md)
- [Documentation](DOCS/README.md)
- [Developer Docs](DOCS/dev/README.md)
- [Security Policy](SECURITY.md)
- [Contributing Guidelines](CONTRIBUTING.md)
- [Contributors](CONTRIBUTORS.md)
- [Code of Conduct](CODE_OF_CONDUCT.md)
