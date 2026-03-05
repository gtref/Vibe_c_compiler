# Build System (Bolt ⚡)

Vibe C features a high-performance build system optimized for developer productivity:

## Performance Engineering

- **Minimizing I/O**: Efficient stack-based `os.scandir` traversal and metadata caching for global headers.
- **Parallel Compilation & Testing**: Uses a worker thread pool to compile multiple source files and run tests simultaneously.
- **Incremental Builds & Tests**: Automatically detects changed source, headers, and libraries to only recompile and rerun what is necessary.
- **Chunked I/O Processing**: String output functions (like `vibe_json_print`) implement a chunked I/O pattern, grouping non-special characters into a single `fwrite` call.
- **Arithmetic Optimization**: The `vibe_xor_cipher` function optimizes throughput by replacing the modulo operator with an incremental index and specializing paths for common key sizes using SWAR.
- **O(1) Concurrency**: The `vibe_thread_pool.h` implementation uses both head and tail pointers for the job queue, ensuring constant-time insertion.
- **Optimized Regex**: Use of combined regular expressions for $O(1)$ pattern matching per line in the security auditor.
- **Binary Hardening**: Automatically applies comprehensive security hardening flags to all compilation and linking steps.
