# Bolt's Performance Journal

## 2024-05-24 - Parallel and Incremental Build System
**Learning:** Python's `ThreadPoolExecutor` is an excellent tool for parallelizing external process calls (like `clang`). In this codebase, the build process was entirely sequential, making it a major bottleneck as the project grows. Adding a coarse-grained incremental check (any header change triggers recompile) is a safe and effective way to speed up the development cycle without the complexity of a full dependency graph.
**Action:** Always check if core loops involve independent external process calls that can be parallelized. Use `os.path.getmtime` for simple but effective incremental build logic.

## 2024-05-24 - Efficient Incremental Scanning
**Learning:** Python's `ThreadPoolExecutor` and `subprocess` calls have significant overhead when called hundreds of times for "no-op" tasks. Moving incremental checks (mtime comparisons) out of the worker function and into a pre-filtering step in the main thread drastically reduces no-op build time (e.g., from 0.22s to 0.08s for 50 files). Combining source file collection and header mtime scanning into a single `os.walk` pass further minimizes expensive I/O operations.
**Action:** In build systems or batch processors, always pre-filter tasks in the main thread before engaging parallel workers. Consolidate file system traversals to minimize 'stat' calls.

## 2024-05-24 - Parallel Test Execution
**Learning:** Parallelizing only the compilation step is not enough if the tasks themselves (e.g., running tests) are also time-consuming. Parallelizing execution while capturing output to prevent interleaving provides a much better developer experience. Incremental builds should extend to tests to avoid redundant compilation of unchanged test files.
**Action:** Always look for opportunities to parallelize independent execution tasks, not just build tasks. Ensure incremental logic is applied consistently across all parts of the toolchain.

## 2025-02-11 - Combined Regex for Multi-Pattern Matching
**Learning:** Performing multiple independent `re.search` calls in a loop over every line of every file is a significant bottleneck ($O(Lines \times Patterns)$). Combining all patterns into a single regex with alternation and using named groups for identification reduces the complexity to $O(Lines)$ and leverages the regex engine's internal optimizations (e.g., Aho-Corasick like matching). This yielded a ~25-30x speedup for the matching logic in this codebase.
**Action:** In scanners or log parsers, always combine multiple patterns into a single pre-compiled regex instead of looping through them.

## 2025-05-14 - os.scandir for Efficient Metadata Access
**Learning:** Using `os.scandir` instead of `os.walk` or manual `os.path.getmtime` calls significantly reduces system calls, especially when traversing large directory trees for file metadata. `DirEntry` objects often cache stat information retrieved during directory listing, making `entry.stat().st_mtime` much faster than `os.path.getmtime(entry.path)`.
**Action:** Use `os.scandir` for any recursive directory traversal that requires file metadata (mtime, size, etc.) to leverage cached stat info.

## 2026-02-13 - Bulk Metadata Collection with os.scandir
**Learning:** In build systems with many files, performing individual 'stat' calls for every source and object file to check for updates is a significant bottleneck. Using a single recursive 'os.scandir' pass to collect all metadata into a dictionary reduces system calls from O(N) to O(D) (number of directories). This optimization is especially effective for 'no-op' builds where most files are already up-to-date.
**Action:** When performing incremental checks across a large set of files, use a bulk directory scan to pre-collect metadata instead of checking files individually in a loop.

## 2024-05-24 - Efficient Line Numbering for Large Scale Scans
**Learning:** When performing regex-based scans on entire file contents (e.g., in a security auditor), using 'content.count('\n', 0, match.start())' inside a loop results in $O(M \times N)$ complexity, where M is the number of matches and N is the file size. This can be significantly optimized to $O(N + M \log N)$ by pre-calculating line start offsets in a single pass and then using 'bisect.bisect_right' to find the line number for each match.
**Action:** Always pre-calculate line offsets when multiple matches within a single file require line number identification.

## 2024-05-24 - Redundant Path and String Operations in Loops
**Learning:** Even with parallelization and incremental builds, redundant string operations and path normalization (e.g., `os.path.relpath`, `os.path.splitext`) within loops over large file sets can become a measurable bottleneck. Pre-calculating these values during the initial file system traversal (`os.scandir`) and passing them through the pipeline eliminates thousands of redundant calls and yields a significant performance boost (~24% for first builds and ~36% for no-op builds in this codebase).
**Action:** Always pre-calculate derived paths (relative paths, object paths, absolute paths) during the discovery phase and avoid recalculating them in hot loops or worker threads.

## 2025-05-20 - Lazy Computations in Batch Processors
**Learning:** In batch processors like security scanners, performing expensive setup (e.g., calculating line offsets for an entire file) for every item regardless of its status is a major bottleneck. Implementing 'Lazy Line Offset Calculation' (only computing offsets if a match is found) reduced the CPU time of the audit command by ~45% for projects with mostly clean files. Combining multiple traversals into a single-pass optimized scan further reduced I/O overhead.
**Action:** In batch tasks, always check if expensive computations or I/O can be deferred until they are absolutely necessary for the specific item being processed.

## 2026-02-18 - [Chunked I/O for String Processing]
**Learning:** Printing strings character-by-character using 'putchar' or 'printf' with a single character format is extremely inefficient due to repeated function call overhead and suboptimal buffering. Grouping non-special characters into chunks and printing them in a single 'fwrite' call significantly reduces this overhead.
**Action:** When processing or escaping strings for output, always accumulate "normal" characters and print them in chunks to maximize I/O throughput.

## 2026-05-22 - [O(1) Job Insertion in Thread Pool]
**Learning:** Maintaining only a head pointer in a job queue results in O(N) insertion time as the entire queue must be traversed to find the tail. This becomes a major bottleneck and increases lock contention as the queue size grows. Adding a tail pointer reduces insertion to O(1) and significantly shortens the critical section.
**Action:** Always use both head and tail pointers for linked-list based queues to ensure O(1) operations and minimize lock hold time.

## 2026-06-15 - [Modulo Operator Costs in Hot Loops]
**Learning:** The modulo operator (%) is relatively expensive as it often translates to a division instruction. In hot loops iterating over large data sets (e.g., XOR ciphers), replacing `i % key_len` with a manually managed incremental index and a simple conditional reset can yield significant performance gains (~34% with -O3, ~3.3x without).
**Action:** Avoid the modulo operator in performance-critical loops where the divisor is constant or changes infrequently; use an incremental counter and an 'if' check instead.

## 2026-06-20 - [Optimized String Escaping with strcspn]
**Learning:** Replacing a manual character-by-character loop with `strcspn` for string scanning can yield significant performance gains (e.g., ~20% faster scanning). Modern C standard libraries often provide SIMD-optimized implementations of `strcspn`. Furthermore, replacing heavy-weight functions like `printf` with manual buffer construction for simple formatting (like hex conversion) can double the speed of string processing hot paths.
**Action:** Use `strcspn` or `strpbrk` for scanning strings for multiple target characters, and avoid `printf` in high-frequency loops when simple manual formatting is possible.

## 2026-07-12 - [Iterative Traversal and Meta-Data Caching]
**Learning:** In complex project structures, recursive directory traversal can incur significant stack overhead and hit depth limits. Transitioning all scanners to an iterative stack-based approach using `os.scandir` ensures robustness. Furthermore, pre-calculating and caching derived metadata (like binary output paths and test names) during the discovery phase, and storing them in a flat list, eliminates thousands of redundant string operations (splitext, join) in subsequent hot loops or parallel workers.
**Action:** Use iterative stack-based scanners for all directory traversals and pre-calculate as much derived file metadata as possible during the initial discovery pass to avoid redundant work in processing loops.
