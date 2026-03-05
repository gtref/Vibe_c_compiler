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
