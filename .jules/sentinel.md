## 2024-05-15 - Incomplete Input Sanitization in Project Initialization
**Vulnerability:** Path traversal via unvalidated `--template` argument.
**Learning:** While project names were sanitized, the template argument was overlooked.
**Prevention:** Apply consistent sanitization to all user-provided strings.

## 2024-05-16 - Inconsistent Sanitization across Commands
**Vulnerability:** Missing project name validation in `run_tests` and `project_status`.
**Learning:** Security fixes often target obvious entry points but miss secondary ones using the same data.
**Prevention:** Audit all locations where user-controlled data is consumed.

## 2026-02-10 - Subprocess Execution in Non-Standard Working Directory
**Vulnerability:** The `update` command performed git operations in the caller's CWD instead of the application's root directory.
**Learning:** Tools that manage themselves (like self-updaters) must explicitly anchor their operations to their own installation path to avoid interfering with user data.
**Prevention:** Always use the `cwd` parameter in `subprocess` calls when the desired operation is context-specific to the application's installation rather than the user's workspace.

## 2026-05-22 - Robustness in Built-in Security Auditing
**Vulnerability:** Brittle string matching in `audit` command led to both false negatives and false positives.
**Learning:** Security tools themselves must be implemented with robust patterns (like regex) to avoid giving a false sense of security. Simple containment checks (`"func(" in line`) are easily bypassed by stylistic variations (e.g., spaces).
**Prevention:** Use regular expressions with boundary markers (`\b`) and handle whitespace variations in all security-scanning logic.

## 2026-05-23 - Insecure LD_LIBRARY_PATH Construction
**Vulnerability:** Prepending paths to `LD_LIBRARY_PATH` without sanitizing the existing value can introduce empty entries (e.g., `path::existing` or `path:`), which the dynamic linker interprets as the current directory (.).
**Learning:** Environmental variable manipulation must be done with awareness of the underlying system's interpretation of special characters like colons.
**Prevention:** Always split environmental variables by their delimiter, filter out empty parts, and then join them back when modifying them.

## 2026-05-24 - Comprehensive Line Auditing and JSON Escaping
**Vulnerability:** Audit tool missed multiple vulnerabilities on a single line; JSON library allowed injection.
**Learning:** Security scanners must use iterative matching (e.g., `finditer`) to be thorough. Library functions that output structured data (like JSON) MUST handle escaping even if they seem "internal" or "simple".
**Prevention:** Always use `finditer` for security scanning patterns. Ensure all data output functions for structured formats implement proper escaping.

## 2026-06-15 - Regex-based Security Audit Bypass
**Vulnerability:** The C security audit regex (`\bfunc\s*\(`) could be bypassed by parenthesizing the function name (e.g., `(printf)("data")`), as the trailing parenthesis prevents the expected `\(` from matching immediately after the function name.
**Learning:** Security scanning patterns that rely on syntactic assumptions (like a function name always being followed by an opening parenthesis) are fragile.
**Prevention:** Use more flexible regex patterns or full AST parsing for security audits. For simple regex checks, focusing on the symbol itself with word boundaries (`\bfunc\b`) is often more robust.

## 2026-06-16 - Syntactic Bypasses in Security Auditing
**Vulnerability:** Security auditors that rely on specific syntactic markers (like a trailing parenthesis for function calls) are easily bypassed by alternative but valid syntax (e.g., `(printf)(buf)`).
**Learning:** In security scanning, it is safer to flag the symbol itself as a whole word (`\bfunc\b`) rather than assuming a specific calling convention. While this may increase false positives (e.g., if a variable shares a name with an unsafe function), it significantly reduces false negatives and forces better naming practices.
**Prevention:** Always use word boundaries and avoid making assumptions about the syntactic context following a sensitive symbol.

## 2026-06-17 - Incomplete Security Audit Coverage
**Vulnerability:** Audit tool skipped internal compiler headers and missed several dangerous function patterns.
**Learning:** A security scanning tool is only as good as its pattern library and its scope; failing to audit internal components or secondary languages (like Python scripts in a C project) can lead to a false sense of security.
**Prevention:** Ensure security tools have a comprehensive pattern list and audit the entire codebase, including bundled libraries and build scripts.

## 2026-06-18 - String Security and Timing Attack Mitigation
**Vulnerability:** Lack of constant-time string comparison in core library.
**Learning:** Standard string comparison functions like `strcmp` are unsuitable for sensitive data as they leak information via execution time.
**Prevention:** Always provide and use constant-time comparison primitives for security-sensitive string operations.

## 2026-06-19 - Uninitialized Memory in Network Structures
**Vulnerability:** Information leakage via uninitialized `sockaddr_in` fields.
**Learning:** Network structures in C often contain padding or reserved fields (like `sin_zero`) that must be explicitly zeroed to prevent leaking stack data to the kernel or the network.
**Prevention:** Always use zero-initialization (e.g., `struct sockaddr_in addr = {0};`) for all network and system structures.

## 2026-06-20 - Unchecked Resource Allocation in Library Headers
**Vulnerability:** NULL pointer dereference and resource leaks in `vibe_thread_pool.h` due to unchecked `malloc` and `pthread_create` calls.
**Learning:** Internal library headers, especially those providing concurrency or memory management primitives, must be audited for standard C safety patterns. Failing to handle resource exhaustion gracefully can lead to critical application failures.
**Prevention:** Always check return values for memory allocation (`malloc`) and thread/synchronization primitives (`pthread_create`, `pthread_mutex_init`). Implement comprehensive cleanup (e.g., using `free` and `pthread_join`) if any part of a multi-stage initialization fails.

## 2026-06-21 - Compile-time Format String Enforcement in Macros
**Vulnerability:** Format string vulnerabilities in variadic macros that wrap `printf`-like functions.
**Learning:** Macros that wrap `printf` can be hardened by using string literal concatenation (`"" __VA_ARGS__`) in the expansion. This trick leverages the C compiler's rule that only string literals can be concatenated this way, effectively forcing the first argument of the macro to be a literal and preventing insecure variable-based format strings at compile-time.
**Prevention:** Always use the `"" __VA_ARGS__` pattern when defining macros that pass arguments directly to the format string position of `printf`, `fprintf`, or similar functions.

## 2026-06-22 - Usability and Suppression in Security Tooling
**Vulnerability:** High false-positive rates in `vcc audit` due to lack of suppression for C/H files.
**Learning:** Security tools that cannot be tuned or silenced for legitimate use cases quickly become counterproductive due to "security fatigue." Providing a mechanism to suppress false positives (like `// nosec`) is as important as the detection logic itself for long-term project security.
**Prevention:** Always implement suppression mechanisms in security scanning tools to allow developers to mark and document legitimate, safe uses of potentially dangerous primitives.

## 2026-06-23 - Uncontrolled Directory Traversal and Resource Management
**Vulnerability:** Compiler followed symlinks during project scanning; thread pool lacked shutdown and validation.
**Learning:** Directory traversal logic must explicitly decide whether to follow symlinks to avoid infinite loops or path traversal. Concurrency primitives must always include lifecycle management (destruction) and input validation to prevent resource exhaustion or DoS.
**Prevention:** Use `follow_symlinks=False` in directory scanning unless symlinks are explicitly required. Ensure all allocated resources (threads, memory) have a clear path to being freed.

## 2026-06-25 - [1.5.5] - Comprehensive Resource Hardening in Concurrency Primitives
**Vulnerability:** Unbounded thread creation, unbounded job queuing, and unchecked job queuing during shutdown in the thread pool.
**Learning:** Concurrency primitives must enforce limits on ALL resources they manage (both threads and memory/queue size) to prevent Denial of Service. Lifecycle state must be checked atomically at all entry points to ensure consistent behavior and prevent leaks.
**Prevention:** Implement strict upper bounds on both worker counts and queue depths. Always verify the current operational state (e.g., shutdown status) while holding the necessary synchronization locks before accepting new work.

## 2026-08-15 - [1.5.9] - Networking Robustness and Resource Isolation
**Vulnerability:** Incomplete networking security (missing port validation, no FD_CLOEXEC, no SO_REUSEADDR) and resource leaks in error paths.
**Learning:** Hardening core utilities requires addressing both external security (resource isolation, DoS) and internal robustness (resource management on failure). Sockets should be marked with FD_CLOEXEC and error paths must explicitly close descriptors to prevent exhaustion.
**Prevention:** Always validate numeric ranges for system parameters (like ports) and ensure all resources (file descriptors, memory) are correctly released in all execution paths, including error returns.
