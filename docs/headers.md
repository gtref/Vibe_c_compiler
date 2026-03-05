# Custom Headers

Vibe C comes with 25 custom headers located in `vibe/include/`. You can include them in your source code using `#include <vibe_xxx.h>`.

### Core Headers
- `vibe_std.h`: Core types and version info. Includes `stdint.h`, `stdbool.h`, and `stdio.h`.
- `vibe_io.h`: Simple printing macros like `vibe_print()` and hardened `vibe_snprintf`/`vibe_vsnprintf`.
- `vibe_math.h`: Math constants and min/max macros.
- `vibe_string.h`: String comparison helpers like `vibe_str_eq()` and secure `vibe_str_copy`.
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
- `vibe_log.h`: Logging macros (`INFO`, `WARN`, `ERROR`).
- `vibe_bench.h`: Micro-benchmarking macro.
- `vibe_test.h`: Simple unit testing assertions.
- `vibe_color.h`: ANSI terminal color codes.
- `vibe_ui.h`: Simple UI/Terminal helpers.
- `vibe_file.h`: Easy file reading utility (Hardened with 10MB size limits).
- `vibe_json.h`: JSON parsing and secure printing with depth tracking.
- `vibe_thread.h`: Simple pthread wrapper.
- `vibe_net.h`: TCP listening and connecting.
- `vibe_crypt.h`: Simple XOR and hashing.
- `vibe_regex.h`: POSIX regex wrapper.
- `vibe_thread_pool.h`: Worker thread pool implementation.
