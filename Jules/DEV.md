# Vibe C Compiler Development Log

## Overview
This log tracks major changes and improvements made to the Vibe C Compiler suite.

## Change-log

### [1.2.0] - 2024-02-07
#### Added
- **Uninstall Command**: Added `uninstall` command to remove the global `vcc` symlink.
- **Templates Command**: Added `templates` command to list available project templates.
- **Headers Command**: Added `headers` command to list all built-in Vibe C headers.
- **Status Command**: Added `status` command to show information about the current project (name, version, type, source files, build artifacts).
- **Minimal Template**: Added a new `minimal` template for lightweight project initialization.
- **Menu Improvements**: Integrated `install`, `uninstall`, `templates`, `headers`, and `status` into the interactive TUI menu.

#### Changed
- **Project Initialization**: The `init` command now supports a `--template` argument (defaults to `basic`).
- **Build Output**: Improved build output to show real-time compilation progress.
- **Version Management**: Centralized version display logic in the `VibeCompiler` class.
- **Menu Header**: The interactive menu now displays the current compiler version.
- **Security Audit Command**: Added `audit` command with built-in checks for C and Python vulnerabilities, plus optional `bandit` and `cppcheck` integration.
- **Update Command**: Added `update` command to perform a `git pull` from the main repository.

#### Fixed
- **Input Sanitization**: Project names are now validated with a strict regex to prevent directory traversal and other injection attacks.
- **JSON Security**: Switched from string replacement to using the `json` library for updating `vibe.json`, preventing malformed JSON or injection.
- **Header Safety**: Improved memory allocation checks and file I/O robustness in `vibe_file.h` and `vibe_json.h`.

## Technical Details

### Uninstall Logic
The uninstall command specifically targets `~/.local/bin/vcc` and removes the symlink if it exists, providing a clean way to remove the global shortcut.

### Template System
Introduced a more flexible template system by allowing the `init` command to pull from different subdirectories in `vibe/templates/`.

### Project Status
The `status` command parses `vibe.json` and scans the `src/` and `build/` directories to provide a quick overview of the project's health and size.

### [1.3.0] - 2024-02-07
#### Added
- **Parallel Compilation**: The build system now compiles C files in parallel using all available CPU cores, significantly reducing build times for multi-file projects.
- **Incremental Builds**: Implemented timestamp-based incremental builds. Only modified files are recompiled, making subsequent builds near-instantaneous.
- **Unified Build Logic**: All project types (executable, shared, static) now share a common, optimized build path.
- **Object File Management**: Build artifacts are now organized with object files stored in `build/obj/`.

#### Changed
- **Build Performance**: Refactored `vibe/core/compiler.py` to use `concurrent.futures.ThreadPoolExecutor` for parallel tasks.
- **Import Optimization**: Moved heavy-use imports (`json`, `re`) to the top level for improved runtime efficiency.
