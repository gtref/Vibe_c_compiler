# Security Log - Vibe C Compiler

This log tracks all security-related changes and audits performed on the Vibe C Compiler.

## [1.5.2] - 2026-06-22

### Security Improvements & Fixes
- **Regex NULL Safety**: (Sentinel 🛡️) Added NULL pointer checks to `vibe_regex_match` in `vibe_regex.h` to prevent potential crashes on unvalidated input.

### Verification
- Verified `vibe_regex_match` handles NULL inputs gracefully without crashing.

## [1.4.9] - 2026-06-18

### Security Improvements & Fixes
- **Timing Attack Mitigation**: (Sentinel 🛡️) Implemented `vibe_str_eq_constant_time` in `vibe_string.h` for secure, constant-time string comparisons.
- **String Robustness**: (Sentinel 🛡️) Added NULL pointer checks to `vibe_str_eq` in `vibe_string.h`.

### Verification
- Verified `vibe_str_eq_constant_time` prevents early-exit timing leaks.
- Verified `vibe_str_eq` handles NULL inputs without crashing.

## [1.4.8] - 2026-06-17

### Security Improvements & Fixes
- **Self-Auditing**: (Sentinel 🛡️) Expanded the `audit` command to include the compiler's own internal headers (`vibe/include/`).
- **Expanded Pattern Detection**: (Sentinel 🛡️) Added 8 new unsafe C functions and 3 Python patterns to the security auditor.
- **Secure Memory Wiping**: (Sentinel 🛡️) Added `vibe_secure_memzero` to `vibe_mem.h` to ensure sensitive data is not left in memory.

### Verification
- Verified `vcc audit` correctly scans and reports issues in `vibe/include/`.
- Verified detection of new unsafe patterns in both C and Python.
- Verified `vibe_secure_memzero` implementation prevents compiler optimization.

## [1.4.7] - 2026-06-16

### Security Improvements & Fixes
- **Audit Tool Reliability**: (Sentinel 🛡️) Switched to word-boundary regex matching in C and Python auditors to prevent syntactic bypasses like `(printf)("...")`.
- **Library Robustness**: (Sentinel 🛡️) Added NULL pointer checks to `vibe_json_new_string`, `vibe_xor_cipher`, and `vibe_read_file` to prevent crashes.
- **Secure JSON Printing**: (Sentinel 🛡️) Enhanced `vibe_json_print` with full JSON-compliant escaping for all control characters (U+0000 to U+001F).

### Verification
- Verified word-boundary matching with test cases containing `(printf)`.
- Verified NULL pointer checks in C headers.
- Verified control character escaping in `vibe_json_print`.

## [1.4.6] - 2026-06-16

### Security Improvements & Fixes
- **Audit System Performance**: (Bolt ⚡) Parallelized the security audit system using `ThreadPoolExecutor`.
- **Audit System Accuracy**: (Bolt ⚡) Optimized scanning engine to use `re.finditer` on entire file contents to ensure all vulnerabilities on a single line are detected.
- **Efficient Line Numbering**: (Bolt ⚡) Implemented $O(\log N)$ line-numbering algorithm using `bisect` and pre-calculated line offsets.

### Verification
- Verified audit performance on large project directories.
- Verified multi-vulnerability detection per line.

## [1.4.5] - 2026-06-15

### Security Improvements & Fixes
- **Binary Hardening**: (Sentinel 🛡️) Implemented comprehensive security hardening flags (-fstack-protector-strong, -D_FORTIFY_SOURCE=2, -Wformat-security, PIE, RELRO) for all compilation and linking processes.

### Verification
- Verified binaries using `hardening-check` or similar tools.

## [1.4.4] - 2026-02-13

### Security Improvements & Fixes
- **Safe Self-Updates**: (Sentinel 🛡️) Fixed a vulnerability in the `update` command where git operations were performed in the caller's CWD. The command is now explicitly anchored to the compiler's installation directory (`self.base_dir`).
- **Environment Safety**: (Sentinel 🛡️) Sanitized `LD_LIBRARY_PATH` construction in `run_tests` to prevent shared library injection via empty entries (interpret as current directory `.`).
- **Enhanced Audit Capabilities**: (Sentinel 🛡️) Upgraded `_internal_c_audit` and `_internal_python_audit` to use regular expressions for more robust detection.
- **Audit Tool Robustness**: (Sentinel 🛡️) Enhanced Python audit tool with word boundaries (`\b`) to prevent false positives and implemented `re.finditer` to catch multiple issues per line.
- **Audit Tool Gaps**: (Sentinel 🛡️) Expanded C audit patterns with `mktemp`, `realpath`, `strtok`, `vfork`, `strncat`, and format string risks (`printf`, `fprintf`, etc.).
- **Audit Tool Gaps**: (Sentinel 🛡️) Expanded Python audit patterns with `yaml.load`, `os.system`, `os.popen`, `os.spawn*`, and `pickle.load/loads`.
- **Secure JSON Printing**: (Sentinel 🛡️) Implemented proper escaping for double quotes and backslashes in `vibe_json.h` and added support for all JSON types.

### Verification
- Verified `LD_LIBRARY_PATH` construction logic.
- Verified audit tool with safe and unsafe test cases.
- Verified `vibe_json_print` with special character strings.

## [1.4.0] - 2024-05-16

### Security Improvements & Fixes
- **Comprehensive Input Validation**: Applied strict regex validation to project names in `test` and `status` commands to prevent path traversal and argument injection via `vibe.json`.
- **Audit Enhancement**: Expanded internal C audit to include `system`, `popen`, and `exec` family functions.
- **Project Sanitization**: Implemented strict regex validation for `--template` and `--arch` arguments in `init` and `build` commands.

## [1.2.0] - 2024-02-07

### Security Audit Findings
- **Path Traversal**: Identified potential path traversal in `init_project` via the `name` argument.
- **JSON Injection**: Found that `vibe.json` was updated using string replacement, which could lead to malformed JSON or injection if a project name contained double quotes.
- **Subprocess Safety**: Verified that `subprocess.run` is used with argument lists, preventing shell injection.
- **C Header Safety**: Found several custom C headers that did not check for `malloc` failures or had incomplete memory freeing logic.

### Improvements & Fixes
- **Input Validation**: Implemented strict regex validation for project names (`^[a-zA-Z0-9_-]+$`).
- **Secure JSON Handling**: Replaced string-based updates of `vibe.json` with the standard `json` library.
- **Robust Subprocesses**: Refined subprocess calls to use absolute paths for executables where appropriate.
- **Memory Safety**: Updated `vibe_file.h` and `vibe_json.h` to include NULL checks for memory allocations and proper error handling for file operations.
- **Recursive Free**: Fully implemented recursive `vibe_json_free` for all JSON types including objects and arrays.
- **Audit Tooling**: Integrated a new `vcc audit` command with built-in, dependency-free pattern matching for common C and Python vulnerabilities. It optionally integrates with `bandit` and `cppcheck`.

---

## Project Navigation

- [Home (README)](../README.md)
- [Documentation](../docs/index.md)
- [Developer Docs](../docs/architecture.md)
- [Security Policy](../SECURITY.md)
- [Contributing Guidelines](../CONTRIBUTING.md)
- [Contributors](../CONTRIBUTORS.md)
- [Code of Conduct](../CODE_OF_CONDUCT.md)
