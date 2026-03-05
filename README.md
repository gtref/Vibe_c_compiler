[![Documentation Status](https://readthedocs.org/projects/vibe-c-compiler/badge/?version=latest)](https://vibe-c-compiler.readthedocs.io/en/latest/?badge=latest)



# Vibe C Compiler (v1.5.11)
---

> **NOTICE:** This repository is the primary **AI-Active Sandbox** for the Vibe C project. 

This environment is currently being integrated and "vibed" by autonomous agents, including **Google Jules** and **CodeRabbit**. Expect high-frequency commits, experimental logic, and AI-driven architectural shifts.

---

## 🛠 Developer Direction

If you are a human collaborator looking to contribute to the stable, governed version of this compiler, please move your development to the official human-led fork:

👉 **[Human_c_compiler @ DOM5-web](https://github.com/DOM5-web/Human_c_compiler)**

### Why the Split?
* **The Hub (This Repo):** Optimized for AI velocity. Jules and CodeRabbit handle the heavy lifting, bug-squashing, and "vibe-checking" of new C implementations.
* **The Fork (DOM5-web):** Optimized for human readability, documentation, and stable releases. This is where the "vibes" are translated into engineering.

---

## 🤖 Active Agents
* **Google Jules:** Automated feature prototyping and systems logic.
* **CodeRabbit:** Continuous AI code review and quality assurance.

---

The Vibe C Compiler is a project management and compilation suite designed to make C development easier and more intuitive than using GCC directly. It wraps around Clang to provide seamless cross-compilation support and project directory management.

>[!NOTE]
> THIS CODE IS AI GENERATED. ESSENTIALLY I RAN A TEST TO SEE WHAT THE AI COULD MAKE WITH A VERY SIMPLE PROMPT. Dont use this for production.

## Features

- **Global Installation**: Use `vcc` command from anywhere after running `install`.
- **Simple CLI**: Easy commands like `init`, `build`, `run`, `test`, `install`, `uninstall`, `audit`, `update`, and `upgrade`.
- **Fast Incremental Builds & Tests**: Optimized for speed with parallel execution, efficient NO-OP checks, and cached header scanning (v1.4.4).
- **Parallel and Incremental Test Execution**: High-speed testing that only recompiles changed tests and runs them concurrently with pre-filtered NO-OP checks (v1.4.2).
- **Binary Hardening**: Automatically applies security hardening flags (stack protection, PIE, RELRO) to all builds and tests (v1.4.5).
- **High-Performance Security Audit**: Features a parallelized, regex-based detection system with $O(\log N)$ line-numbering for rapid vulnerability scanning (v1.4.6).
- **Audit Reliability**: Improved scanner accuracy with word-boundary matching to prevent syntactic bypasses (v1.4.7).
- **Expanded Auditing**: Includes the compiler's own internal headers in security scans and detects more unsafe patterns (v1.4.8).
- **Network Security Hardening**: Implemented `SOMAXCONN` backlogs and zero-initialization of network structures to mitigate DoS and information leakage (v1.5.0).
- **Thread Pool Robustness**: Added comprehensive error handling (malloc, pthread_create), atomic initialization/cleanup, and $O(1)$ job insertion to the worker thread pool (v1.5.1).
- **Thread Pool Hardening**: Enforces a strict 1024 thread limit and shutdown-aware job rejection to mitigate DoS and prevent memory leaks (v1.5.5-v1.5.10).
- **Constant-Time String Security**: Hardened `vibe_str_eq_constant_time` to avoid string length leakage via a single-pass implementation (v1.5.6).
- **JSON Stack Hardening**: Implemented recursive depth tracking and hardened printing macros in JSON serialization (v1.5.10).
- **File I/O Hardening**: Enforced 10MB size limits and added robust error handling for `vibe_read_file` to mitigate OOM/DoS (v1.5.10).
- **Hardened String and I/O Primitives**: Implemented `vibe_snprintf`, `vibe_vsnprintf`, and `vibe_str_copy` to eliminate format string and buffer overflow risks (v1.5.11).
- **Regex NULL Safety**: Implemented NULL pointer checks in `vibe_regex_match` to prevent application crashes (v1.5.2).
- **Optimized XOR Cipher**: Replaced modulo operator with incremental indexing and specialized SWAR paths for significantly improved performance (v1.5.2-v1.5.7).
- **Secure JSON Printing**: Built-in JSON printing with full control character escaping and high-performance chunked I/O (v1.4.7).
- **Compile-time Format String Hardening**: Enforces string literals in printing macros to prevent format string injection at the source (v1.5.1).
- **Secure Memory Primitives**: Added `vibe_secure_memzero` to ensure sensitive data can be securely wiped from memory (v1.4.8).
- **Library Robustness**: Core headers include NULL pointer checks for increased stability (v1.4.7-v1.5.10).
- **Project Management**: Manages your project structure (`src/`, `build/`, `vibe.json`).
- **Interactive Menu**: A simple TUI for those who prefer menus.
- **High-Performance Scanning**: Uses `os.scandir` for rapid file discovery across all commands (v1.4.4).
- **Cross-Compilation**: Easily target any architecture supported by Clang.
- **Library Support**: Effortlessly create static and shared libraries.
- **25 Custom Headers**: Built-in headers for advanced features (SIMD, Math, Networking, UI, etc.).

## Quick Start

1. **Initialize a project**:
   ```bash
   ./vibe_c_compiler init my_cool_project
   cd my_cool_project
   ```

2. **Install Globally**:
   ```bash
   ./vibe_c_compiler install
   ```
   Now you can use `vcc` instead of `./vibe_c_compiler`.

3. **Build and Run**:
   ```bash
   vcc run
   ```

4. **Uninstall Globally**:
   ```bash
   vcc uninstall
   ```

5. **Open the Menu**:
   ```bash
   vcc menu
   ```

6. **Run Security Audit**:
   ```bash
   vcc audit
   ```

7. **Update Compiler**:
   ```bash
   vcc update  # or vcc upgrade
   ```

## Documentation

See [Documentation](docs/index.md) for full details on headers and advanced usage.

---

## Project Navigation

- [Home (README)](README.md)
- [Documentation](docs/index.md)
- [Developer Docs](docs/architecture.md)
- [Security Policy](SECURITY.md)
- [Contributing Guidelines](CONTRIBUTING.md)
- [Contributors](CONTRIBUTORS.md)
- [Code of Conduct](CODE_OF_CONDUCT.md)
