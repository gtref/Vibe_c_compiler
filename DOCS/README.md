# Vibe C Compiler Documentation

## CLI Commands

- `install`: Installs the compiler globally as the `vcc` command.
- `uninstall`: Removes the global `vcc` symlink from `~/.local/bin`.
- `init <name>`: Creates a new Vibe C project.
  - `--template <name>`: Use a specific project template (e.g., `basic`, `minimal`).
- `build`: Compiles the project using parallel and incremental builds. It optimizes the linking phase by avoiding a redundant link if no source files were recompiled and the existing binary is up-to-date.
  - `--arch <target>`: Specify target architecture (e.g., `aarch64-linux-gnu`).
  - `--lib <type>`: Build as `static` or `shared` library.
- `run`: Builds and executes the project.
- `test`: Automatically finds, compiles (in parallel), and runs C tests in the `tests/` directory.
- `clean`: Removes the `build/` directory.
- `menu`: Opens the interactive menu.
- `version`: Shows the current version.
- `audit`: Runs a security audit on the compiler and your project (now with self-auditing of internal headers as of v1.4.8).
- `update` / `upgrade`: Updates the Vibe C Compiler from its GitHub repository.
- `status`: Displays current project information (name, version, type, sources, and build artifacts).
- `headers`: Lists all available Vibe C custom headers.
- `templates`: Lists all available project templates.

## Custom Headers

Vibe C comes with 25 custom headers located in `vibe/include/`. You can include them in your source code using `#include <vibe_xxx.h>`.

### Core Headers
- `vibe_std.h`: Core types and version info. Includes `stdint.h`, `stdbool.h`, and `stdio.h`. (v1.5.8: Bumped version)
- `vibe_io.h`: Simple printing macros like `vibe_print()` (now with compile-time format string hardening).
- `vibe_math.h`: Math constants and min/max macros.
- `vibe_string.h`: String comparison helpers like `vibe_str_eq()` and `vibe_str_eq_constant_time()` (v1.5.6: Hardened constant-time comparison).
- `vibe_sys.h`: OS detection macros (`VIBE_OS_LINUX`, `VIBE_OS_WINDOWS`, `VIBE_OS_MACOS`).
- `vibe_arg.h`: Simple command-line argument parsing utilities (`vibe_arg_has`, `vibe_arg_get`).

### Architecture & Optimization
- `vibe_arch.h`: Top-level architecture include.
- `vibe_simd.h`: SIMD intrinsics wrapper.
- `vibe_x86_64.h`: x86_64 specific definitions.
- `vibe_arm64.h`: ARM64 specific definitions.
- `vibe_rv64.h`: RISC-V specific definitions.

### Utilities
- `vibe_mem.h`: Memory allocation wrappers (now with `vibe_secure_memzero`).
- `vibe_time.h`: High-resolution monotonic timer.
- `vibe_log.h`: Logging macros (`INFO`, `WARN`, `ERROR`) (now with compile-time format string hardening).
- `vibe_bench.h`: Micro-benchmarking macro.
- `vibe_test.h`: Simple unit testing assertions.
- `vibe_color.h`: ANSI terminal color codes.
- `vibe_ui.h`: Simple UI/Terminal helpers.
- `vibe_file.h`: Easy file reading utility (now with NULL checks).
- `vibe_json.h`: JSON parsing and secure printing with depth tracking (v1.5.8: Hardened printing macros).
- `vibe_thread.h`: Simple pthread wrapper.
- `vibe_net.h`: TCP listening and connecting (now with `SOMAXCONN` hardening, zero-initialization, and NULL checks).
- `vibe_crypt.h`: Simple XOR and hashing (now with NULL checks and optimized XOR hot loop).
- `vibe_regex.h`: POSIX regex wrapper (now with NULL checks).
- `vibe_thread_pool.h`: Worker thread pool implementation (now with robust error handling, atomic initialization, and race condition hardening).

## Project Configuration

Every project contains a `vibe.json` file:

```json
{
  "name": "my_project",
  "version": "1.0.0",
  "type": "executable"
}
```

## Cross-Compilation

To cross-compile for another architecture, use the `--arch` flag:

```bash
./vibe_c_compiler build --arch aarch64-linux-gnu
```

This uses Clang's `-target` flag internally.

## Security Audit

Vibe C includes a built-in security audit command to help identify potential vulnerabilities:

```bash
vcc audit
```

This command performs:
1.  **Internal Audit**: Built-in pattern-based checks for common C and Python security issues. It features a high-performance parallelized scanner (v1.4.6) that uses an optimized regex-based detection system with $O(\log N)$ line-numbering and $O(1)$ matching complexity per line. As of v1.4.8, it also performs self-auditing of the compiler's own internal headers and includes detection for an expanded set of 8 additional C functions and 3 Python patterns. It scans `src/`, `vibe/`, and `tests/` for unsafe functions and Python anti-patterns.
2.  **Bandit** (Optional): A deeper security linter for Python (runs only if `bandit` is installed).
3.  **Cppcheck** (Optional): A more advanced static analysis tool for C/C++ (runs only if `cppcheck` is installed).

It is recommended to run this command regularly during development.

## Project Templates

Vibe C supports different project templates when initializing a project:

- `basic`: Standard project structure with `src/`, `build/`, and a sample `main.c`.
- `minimal`: A lightweight template for small projects.

Use the `--template` flag with the `init` command:

```bash
vcc init my_project --template minimal
```

You can list all available templates with `vcc templates`.

## Build System (Bolt ⚡)

Vibe C features a high-performance build system optimized for developer productivity:

- **Parallel Compilation & Testing**: Uses a worker thread pool to compile multiple source files and run tests simultaneously (v1.4.4). The thread pool features $O(1)$ job insertion to minimize lock contention (v1.5.1).
- **Parallelized Security Audit**: The security audit system is parallelized across multiple cores for rapid project-wide scanning, including internal header checks (v1.4.8).
- **High-Performance JSON Printing**: Optimized with chunked I/O to significantly reduce system call overhead when printing strings (v1.4.7).
- **Optimized XOR Cipher**: Uses expanded word-sized specializations for common key sizes (1, 2, 4, 8, 16 bytes) to maximize throughput in the XOR hot loop (v1.5.7).
- **Incremental Builds & Tests**: Automatically detects changed source, headers, and libraries to only recompile and rerun what is necessary.
- **Binary Hardening**: Automatically applies comprehensive security hardening flags (e.g., Stack Protector, PIE, RELRO) to all compilation and linking steps (v1.4.5).
- **Thread Pool Robustness**: `vibe_thread_pool.h` includes comprehensive error handling for `malloc` and `pthread` failures, with atomic cleanup logic to prevent resource leaks (v1.5.1).
- **Optimized Scanning**: Efficient `os.scandir` scanning, cached header `mtime`, and $O(1)$ multi-pattern matching ensure that builds, tests, and security audits are high-performance (v1.4.4+).

---

## Project Navigation

- [Home (README)](../README.md)
- [Documentation](README.md)
- [Developer Docs](dev/README.md)
- [Security Policy](../SECURITY.md)
- [Contributing Guidelines](../CONTRIBUTING.md)
- [Contributors](../CONTRIBUTORS.md)
- [Code of Conduct](../CODE_OF_CONDUCT.md)
