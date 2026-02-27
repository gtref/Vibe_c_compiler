# Sentinel Security Log 🛡️

## 2026-06-25 - [1.5.5] - Thread Pool Resource Hardening (Threads and Jobs)

### 🔍 Found
- **Resource Exhaustion (DoS) - Threads**: `vibe_thread_pool_create` lacked an upper bound on the number of threads, allowing system resource exhaustion or potential integer overflow in allocation size.
- **Resource Exhaustion (DoS) - Jobs**: The thread pool lacked a limit on the number of queued jobs. An attacker could flood the system with jobs, leading to unbounded memory consumption and application crashes.
- **Race Condition & Memory Leak**: `vibe_thread_pool_add_job` did not check the `shutdown` flag. If a job was added while the pool was being destroyed, it could lead to memory leaks and unpredictable behavior.

### 🎯 Impact
- **Denial of Service**: Excessive thread creation or an unbounded job queue can crash the application or the entire system via OOM (Out Of Memory).
- **Unstable Shutdown**: Race conditions during shutdown can lead to crashes or resource leakage.

### 🔧 Fix
- **Thread Limiting**: Enforced a maximum of 1024 threads in `vibe_thread_pool_create`.
- **Job Queue Limiting**: Implemented a `max_queue_size` (default 65536) in the thread pool and enforced it in `vibe_thread_pool_add_job`.
- **Shutdown & Limit Rejection**: Updated `vibe_thread_pool_add_job` to reject and free jobs if the pool is shutting down OR if the queue is full.

### ✅ Verification
- Enhanced `tests/test_thread_pool_security.c` to verify that oversized pool creation is rejected, jobs are rejected during shutdown, and the job queue limit is correctly enforced.
- Confirmed all security and functional tests pass.

## 2026-06-22 - [1.5.3] - Security Auditor Refinement and Suppression Support

### 🔍 Found
- **Brittle Security Auditing**: The `vcc audit` tool was flagging the compiler's own hardened macros (`vibe_print`, `vibe_error`) as vulnerabilities, leading to excessive noise and masking real issues.
- **Lack of Suppression Mechanism**: Unlike the Python auditor which supports `# nosec`, the C/H auditor had no way to suppress false positives, forcing developers to either ignore the audit output or rename legitimate functions.

### 🎯 Impact
- **Security Fatigue**: High false-positive rates in security tools often lead to developers ignoring all warnings, including critical ones.
- **Inaccurate Audits**: Flagging hardened macros as "unsafe" provided an inaccurate view of the codebase's security posture.

### 🔧 Fix
- **Suppression Support**: Enhanced `_audit_file` in `vibe/core/compiler.py` to support `// nosec` and `/* nosec */` comments in C and C++ (header) files.
- **Auditor Tuning**: Removed `vibe_print` and `vibe_error` from the `_C_UNSAFE_FUNCS` list, as these are intentionally hardened by the library to prevent format string vulnerabilities at compile-time.
- **Noise Reduction**: Applied `nosec` suppression to legitimate uses of `printf` and `fprintf` within the core library headers (`vibe_io.h`, `vibe_log.h`, `vibe_test.h`, etc.).

### ✅ Verification
- Ran `vcc audit` on the entire codebase; confirmed that the issue count dropped from 23 to 0.
- Verified that legitimate unsafe functions (like a raw `printf` without `nosec`) are still correctly detected.
- Confirmed all functional tests pass after library modifications.

## 2026-06-21 - Compile-time Format String Hardening

### 🔍 Found
- **Format String Vulnerabilities**: The `vibe_print` and `vibe_error` macros in `vibe_io.h` were simple wrappers around `printf` and `fprintf`. They allowed passing variables as the first argument, which could lead to format string vulnerabilities if those variables contained user-controlled data.
- **Audit Tool Warnings**: The internal `vcc audit` tool flagged these macros as potential security risks but they remained unhardened in the core library.

### 🎯 Impact
- **Arbitrary Code Execution/Information Leakage**: An attacker could exploit a format string vulnerability to read from or write to arbitrary memory locations.
- **Syntactic Bypass**: While the compiler flags `-Wformat-security` were enabled, some environments or older compilers might not enforce this, leaving applications vulnerable.

### 🔧 Fix
- **Hardened Macros**: Redefined `vibe_print` and `vibe_error` in `vibe/include/vibe_io.h` to use string literal concatenation: `#define vibe_print(...) printf("" __VA_ARGS__)`.
- **Compile-time Enforcement**: This change forces the first argument of these macros to be a string literal. If a variable is passed as the first argument, the code will fail to compile, effectively eliminating the vulnerability at the source.

### ✅ Verification
- Verified that previously vulnerable code (passing a variable to `vibe_print`) now fails to compile with a clear error.
- Verified that legitimate uses (passing a string literal format) continue to work as expected.
- Ran full security audit and confirmed no regressions.

## 2026-06-20 - Thread Pool Robustness and Memory Safety

### 🔍 Found
- **Unchecked Memory Allocations**: `vibe_thread_pool_create` and `vibe_thread_pool_add_job` in `vibe_thread_pool.h` failed to check the return values of `malloc`. This could lead to NULL pointer dereferences and application crashes under memory pressure.
- **Unchecked Thread Creation**: The thread pool initialization loop did not check the return value of `pthread_create`, potentially leading to a partially initialized pool or crashes if system resources were exhausted.
- **Missing Cleanup Logic**: In the event of an initialization failure (e.g., `pthread_create` failing halfway through), the thread pool did not correctly clean up previously allocated memory or join already started threads.

### 🎯 Impact
- **Application Instability**: Unchecked `malloc` and `pthread_create` calls can lead to segmentation faults and unpredictable behavior.
- **Resource Leaks**: Failed initialization could leave orphan threads or leaked memory if not properly handled.

### 🔧 Fix
- **NULL Safety**: Added comprehensive checks for all `malloc` calls in `vibe_thread_pool.h`.
- **Error Handling**: Implemented checks for `pthread_mutex_init`, `pthread_cond_init`, and `pthread_create`.
- **Atomic Initialization**: Added a cleanup mechanism that shuts down and joins any partially started threads and frees all resources if any part of the pool initialization fails.

### ✅ Verification
- Created `tests/test_thread_pool_functional.c` and verified that the thread pool correctly initializes and executes jobs.
- Manually reviewed error paths to ensure proper cleanup on allocation or thread creation failure.

## 2026-06-19 - Network Security, Robustness, and DoS Mitigation

### 🔍 Found
- **Uninitialized Memory Leakage**: `vibe_net_listen` and `vibe_net_connect` in `vibe_net.h` failed to zero-initialize `sockaddr_in` structures. This could lead to leaking uninitialized stack data (e.g., in the `sin_zero` field) to the kernel or over the network.
- **NULL Pointer Dereferences**: `vibe_simple_hash` in `vibe_crypt.h` and `vibe_net_connect` in `vibe_net.h` lacked NULL pointer checks on their string arguments, leading to application crashes.
- **DoS Risk**: The default backlog for `listen()` in `vibe_net_listen` was set to a very low value (3), making applications vulnerable to connection exhaustion/SYN flood attacks.

### 🎯 Impact
- **Information Leakage**: Sensitive data previously residing on the stack could be exposed.
- **Denial of Service**: Crashing the application via NULL pointers or exhausting connections due to small backlogs.

### 🔧 Fix
- **Safe Initialization**: Used `{0}` to ensure all fields of `sockaddr_in` are zeroed.
- **Defensive Programming**: Added NULL pointer checks to `vibe_simple_hash` and `vibe_net_connect`.
- **Hardening**: Increased the `listen` backlog to `SOMAXCONN` for better resilience.

### ✅ Verification
- Created a reproduction test case that confirmed the `vibe_simple_hash(NULL)` crash; verified it now passes.
- Verified all other library tests continue to pass.

## 2026-06-16 - Enhanced Library Robustness and Audit Tool Reliability

### 🔍 Found
- **JSON Injection & Invalid Output**: The `vibe_json_print` function in `vibe_json.h` only escaped double quotes and backslashes, but failed to escape control characters (U+0000 to U+001F). This resulted in invalid JSON output and potential log injection when strings contained characters like newlines or tabs.
- **Audit Tool Bypass**: The security auditor's C patterns required a trailing opening parenthesis (e.g., `printf\s*\(`), which could be bypassed by parenthesizing the function name (e.g., `(printf)("data")`).
- **Missing NULL Pointer Safety**: Several core library functions (`vibe_json_new_string`, `vibe_xor_cipher`, `vibe_read_file`) lacked NULL pointer checks on their inputs, leading to potential crashes in user applications.

### 🎯 Impact
- **Security Bypass**: Developers relying on the audit tool might miss critical vulnerabilities if they use alternative syntax.
- **Data Corruption/Injection**: Malicious or unexpected data in JSON strings could break downstream parsers or pollute logs.
- **Application Instability**: Improper handling of NULL pointers could lead to segmentation faults in projects built with the compiler.

### 🔧 Fix
- **JSON Escaping**: Implemented full JSON-compliant escaping for all control characters in `vibe/include/vibe_json.h`.
- **Auditor Robustness**: Updated `vibe/core/compiler.py` to use word boundaries (`\b`) instead of requiring parentheses, ensuring the auditor catches all symbol references regardless of syntax.
- **Defensive Programming**: Added NULL pointer checks to critical functions in `vibe_json.h`, `vibe_crypt.h`, and `vibe_file.h`.

### ✅ Verification
- Verified `vibe_json_print` output with strings containing newlines and tabs; they are now correctly escaped as `\n` and `\t`.
- Verified `vcc audit` now detects `(printf)` and other parenthesized function calls.
- Confirmed library functions return early or handle NULL inputs gracefully.

## 2026-06-18 - String Security and NULL Robustness in Core Library

### 🔍 Found
- **NULL Pointer Dereference**: `vibe_str_eq` in `vibe_string.h` lacked NULL pointer checks, leading to crashes when comparing NULL strings.
- **Timing Attack Vulnerability**: The library lacked a constant-time string comparison function, making applications vulnerable to timing attacks when comparing sensitive data like tokens or passwords.

### 🎯 Impact
- **Denial of Service**: Passing NULL to string comparison functions could crash the application.
- **Information Leakage**: Traditional `strcmp` returns early upon finding a difference, leaking information about the matching prefix of a secret string.

### 🔧 Fix
- **Robustness**: Added NULL pointer checks to `vibe_str_eq` in `vibe/include/vibe_string.h`.
- **Constant-Time Comparison**: Implemented `vibe_str_eq_constant_time` in `vibe/include/vibe_string.h`.

### ✅ Verification
- Created `tests/security_test.c` which verifies that `vibe_str_eq` no longer crashes on NULL inputs and that `vibe_str_eq_constant_time` correctly compares strings.

## 2026-06-17 - Comprehensive Auditing and Secure Memory Primitives

### 🔍 Found
- **Incomplete Audit Scope**: The `vcc audit` tool only scanned user project source and tests, but skipped the compiler's own internal headers (`vibe/include/`). This left potential vulnerabilities in the core library unmonitored.
- **Limited Vulnerability Patterns**: The auditor missed several dangerous C functions (`strncpy`, `snprintf`, `syslog`, `setuid`, etc.) and Python patterns (`pickle.loads`, `marshal.load`).
- **Lack of Secure Memory Wiping**: The library lacked a primitive for securely clearing sensitive data (like encryption keys) from memory, which is a common requirement for high-security applications to prevent data leakage after use.

### 🎯 Impact
- **Blind Spots**: Vulnerabilities in standard headers provided by Vibe could be overlooked.
- **False Sense of Security**: Missing common unsafe patterns like `syslog` format string vulnerabilities or `strncpy` null-termination issues reduced the tool's effectiveness.
- **Data Persistence**: Without a secure memzero, "deleted" sensitive information might persist in RAM, where it could be harvested by other processes or after a crash.

### 🔧 Fix
- **Self-Auditing**: Updated `run_audit` in `vibe/core/compiler.py` to include the `vibe/include` directory in its security checks.
- **Expanded Pattern Library**: Added 8 new C functions and 3 new Python patterns to the internal auditors in `compiler.py`.
- **Security Primitives**: Implemented `vibe_secure_memzero` in `vibe/include/vibe_mem.h` using a `volatile` pointer to prevent compiler optimizations from skipping memory clearing.

### ✅ Verification
- Verified that `vcc audit` now correctly flags issues in `vibe/include`.
- Verified detection of `strncpy`, `syslog`, and `pickle.loads` in test files.
- Confirmed `vibe_secure_memzero` correctly implementation in the header.

## 2026-06-22 - [1.5.2] - Regex NULL Safety and Robustness

### 🔍 Found
- **NULL Pointer Dereference in Regex**: The `vibe_regex_match` function in `vibe_regex.h` lacked NULL pointer checks for its `pattern` and `text` arguments. This would lead to application crashes (Denial of Service) if NULL was passed to either argument.

### 🎯 Impact
- **Denial of Service**: Malicious or buggy code calling the regex library with unvalidated inputs could crash the entire application process.

### 🔧 Fix
- **Defensive Programming**: Added explicit NULL pointer checks at the entry of `vibe_regex_match` in `vibe/include/vibe_regex.h`. The function now returns `false` gracefully if either the pattern or the text is NULL.

### ✅ Verification
- Updated `tests/security_test.c` with new test cases covering NULL inputs for `vibe_regex_match`.
- Confirmed that all security tests pass and no crashes occur when passing NULL to the regex engine.

## 2026-06-23 - [1.5.4] - Thread Pool Hardening and Symlink Traversal Protection

### 🔍 Found
- **Thread Pool Denial of Service (DoS)**: The `vibe_thread_pool_create` function allowed creating a pool with 0 or negative threads. A pool with 0 threads would accept jobs but never process them, leading to an unbounded queue and potential memory exhaustion.
- **Resource Leaks**: The thread pool lacked a destruction function, making it impossible to cleanly shut down workers and free resources, leading to long-term memory and thread leakage.
- **Compiler Symlink Traversal**: The compiler's directory traversal functions (used for building, testing, and auditing) followed symbolic links by default. This could be exploited to cause infinite recursion (via circular symlinks) or to trick the compiler into scanning/accessing files outside the project directory.

### 🎯 Impact
- **Availability**: Resource exhaustion via unbounded queues or leaked threads could lead to Denial of Service.
- **Stability**: Infinite recursion during file scanning could crash the compiler or consume excessive system resources.
- **Information Leakage/Path Traversal**: Following symlinks could allow the compiler to access sensitive files if a malicious symlink is introduced into the project.

### 🔧 Fix
- **Input Validation**: Added a check to `vibe_thread_pool_create` to ensure `num_threads > 0`.
- **Safe Shutdown**: Implemented `vibe_thread_pool_destroy` in `vibe_thread_pool.h` which broadcasts a shutdown signal, joins all worker threads, and clears the remaining job queue.
- **Symlink Hardening**: Updated all `os.scandir` loops in `vibe/core/compiler.py` to use `entry.is_dir(follow_symlinks=False)`, ensuring the compiler only traverses actual directories within the project.

### ✅ Verification
- Verified that `vibe_thread_pool_create(0)` now returns `NULL`.
- Confirmed `vibe_thread_pool_destroy` correctly cleans up all threads and memory in `tests/test_thread_pool_functional.c`.
- Verified all compiler tests and security audits pass with the new symlink restrictions.

## 2026-06-26 - [1.5.6] - Constant-time Comparisons and JSON Depth Hardening

### 🔍 Found
- **Side-Channel Vulnerability (Timing Attack)**: `vibe_str_eq_constant_time` used `strlen()` to calculate lengths before comparison. This leaked the lengths of secret strings via timing side-channels, as `strlen`'s execution time depends on the null terminator's position.
- **Denial of Service (DoS) - Stack Overflow**: `vibe_json_print` used unbounded recursion for nested JSON objects and arrays. An attacker could provide a deeply nested JSON structure to cause a stack overflow and crash the application.
- **Audit Tool False Positives**: Safe uses of `printf` in `vibe_json.h` were being flagged by the security auditor.

### 🎯 Impact
- **Information Leakage**: Secret lengths could be recovered by an attacker.
- **Application Availability**: Stack exhaustion leads to immediate crashes.
- **Security Fatigue**: False positives in audit tools can mask real issues.

### 🔧 Fix
- **Safe Comparison**: Refactored `vibe_str_eq_constant_time` to use a single-pass loop without `strlen()`, minimizing length-based branching.
- **Recursion Depth Limit**: Implemented a `VIBE_JSON_MAX_DEPTH` (128) limit in `vibe_json_print`. If exceeded, the printer outputs a placeholder instead of recursing deeper.
- **Auditor Suppression**: Added `// nosec` to safe `printf` calls in the library.

### ✅ Verification
- Updated `tests/security_test.c` with more edge cases for constant-time string comparison.
- Ran the full test suite and security audit; all tests passed and the library is now cleaner in the audit output.

---

## Project Navigation

- [Home (README)](../README.md)
- [Documentation](../DOCS/README.md)
- [Developer Docs](../DOCS/dev/README.md)
- [Security Policy](../SECURITY.md)
- [Contributing Guidelines](../CONTRIBUTING.md)
- [Contributors](../CONTRIBUTORS.md)
- [Code of Conduct](../CODE_OF_CONDUCT.md)
