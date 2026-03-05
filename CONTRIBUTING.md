# Contributing to Vibe C Compiler

Thank you for your interest in contributing to the Vibe C Compiler! We welcome contributions from everyone.

## Getting Started

1.  **Fork the repository** on GitHub.
2.  **Clone your fork** locally:
    ```bash
    git clone https://github.com/gtref/Vibe_c_compiler.git
    cd Vibe_c_compiler
    ```
3.  **Ensure you have dependencies**:
    - Python 3.7+
    - Clang
    - Git

## Development Workflow

### Making Changes
- Create a new branch for your feature or bug fix:
  ```bash
  git checkout -b my-new-feature
  ```
- Make your changes in the codebase.
- Ensure you follow the coding standards (see below).

### Testing
- Run the internal test suite:
  ```bash
  ./vibe_c_compiler test
  ```
- Run a security audit:
  ```bash
  ./vibe_c_compiler audit
  ```
- If you add a new feature, add a corresponding test in the `tests/` directory.

### Pull Request Process
1.  **Push your changes** to your fork.
2.  **Open a Pull Request** against the `main` branch of the original repository.
3.  Provide a clear description of your changes and why they are needed.
4.  Wait for review from the maintainers.

## Coding Standards

### Python
- Follow PEP 8 guidelines.
- Use descriptive variable and function names.
- Document complex logic with comments.
- All new features should be integrated into the `VibeCompiler` class in `vibe/core/compiler.py`.

### C (Headers)
- Use the `vibe_` prefix for all public functions and macros to avoid name collisions.
- Include proper NULL checks and error handling.
- Follow the existing style in `vibe/include/`.

### Documentation
- If you add a CLI command, update `docs/index.md` and `README.md`.
- If you add a new header, update the header list in `docs/headers.md`.
- Major changes should be recorded in `Jules/DEV.md`.

## Philosophy

- **Bolt ⚡**: Speed is a feature. Keep compilation and scanning efficient.
- **Sentinel 🛡️**: Security is paramount. Always validate user input and avoid unsafe C functions.

## Questions?

If you have any questions, feel free to open an issue or contact the core team members listed in `CONTRIBUTORS.md`.

---

## Project Navigation

- [Home (README)](README.md)
- [Documentation](docs/index.md)
- [Developer Docs](docs/architecture.md)
- [Security Policy](SECURITY.md)
- [Contributing Guidelines](CONTRIBUTING.md)
- [Contributors](CONTRIBUTORS.md)
- [Code of Conduct](CODE_OF_CONDUCT.md)
