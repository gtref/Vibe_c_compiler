# Getting Started

## Installation

1. **Clone the repository**:
   ```bash
   git clone https://github.com/DOM5-web/vibe_c_compiler.git
   cd vibe_c_compiler
   ```

2. **Install Globally**:
   ```bash
   ./vibe_c_compiler install
   ```
   Now you can use `vcc` instead of `./vibe_c_compiler`.

## Creating a New Project

1. **Initialize a project**:
   ```bash
   vcc init my_cool_project
   cd my_cool_project
   ```

2. **Build and Run**:
   ```bash
   vcc run
   ```

## Project Templates

Vibe C supports different project templates when initializing a project:

- `basic`: Standard project structure with `src/`, `build/`, and a sample `main.c`.
- `minimal`: A lightweight template for small projects.

Use the `--template` flag with the `init` command:

```bash
vcc init my_project --template minimal
```

You can list all available templates with `vcc templates`.
