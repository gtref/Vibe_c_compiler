# Welcome to Vibe C Compiler

The Vibe C Compiler is a project management and compilation suite designed to make C development easier and more intuitive than using GCC directly. It wraps around Clang to provide seamless cross-compilation support and project directory management.

## Key Features

- **Global Installation**: Use `vcc` command from anywhere after running `install`.
- **Simple CLI**: Easy commands like `init`, `build`, `run`, `test`, `install`, `uninstall`, `audit`, `update`, and `upgrade`.
- **Fast Incremental Builds & Tests**: Optimized for speed with parallel execution, efficient NO-OP checks, and cached header scanning.
- **Binary Hardening**: Automatically applies security hardening flags (stack protection, PIE, RELRO) to all builds and tests.
- **High-Performance Security Audit**: Features a parallelized, regex-based detection system for rapid vulnerability scanning.
- **Custom Headers**: Built-in headers for advanced features (SIMD, Math, Networking, UI, etc.).

## CLI Commands

- `install`: Installs the compiler globally as the `vcc` command.
- `uninstall`: Removes the global `vcc` symlink from `~/.local/bin`.
- `init <name>`: Creates a new Vibe C project.
  - `--template <name>`: Use a specific project template (e.g., `basic`, `minimal`).
- `build`: Compiles the project using parallel and incremental builds.
  - `--arch <target>`: Specify target architecture (e.g., `aarch64-linux-gnu`).
  - `--lib <type>`: Build as `static` or `shared` library.
- `run`: Builds and executes the project.
- `test`: Automatically finds, compiles (in parallel), and runs C tests in the `tests/` directory.
- `clean`: Removes the `build/` directory.
- `menu`: Opens the interactive menu.
- `version`: Shows the current version.
- `audit`: Runs a security audit on the compiler and your project.
- `update` / `upgrade`: Updates the Vibe C Compiler from its GitHub repository.
- `status`: Displays current project information (name, version, type, sources, and build artifacts).
- `headers`: Lists all available Vibe C custom headers.
- `templates`: Lists all available project templates.

## Quick Links

- [Getting Started](getting-started.md)
- [Architecture](architecture.md)
- [Custom Headers](headers.md)
- [Security Audit](security.md)
- [Build System (Bolt ⚡)](bolt.md)
