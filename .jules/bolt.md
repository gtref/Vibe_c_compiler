[Output truncated for brevity]

## 2026-03-02 - [SWAR for DJB2 Hashing]
**Learning:** SWAR (SIMD Within A Register) optimization for hashing functions like DJB2 can yield significant performance gains (~17-19%) on large strings. Key requirements include: 1) Aligning the input pointer to the machine word size (e.g., 8 bytes) to avoid unaligned access penalties; 2) Using a non-branching bitmask trick \((v - 0x01...ULL) & ~v & 0x80...ULL\) to detect null terminators within a word; 3) Accessing bytes from the loaded word using a portable \((const\ unsigned\ char*)\) cast to maintain endian neutrality; and 4) Unrolling the hash updates for the entire word to maximize throughput.
**Action:** When optimizing string-processing loops (hashing, checksums, searching), always consider word-sized processing (SWAR) with explicit alignment handling and branch-free terminator detection.

## 2026-07-28 - [Specialization for 32-byte keys in XOR Ciphers]
**Learning:** Specializing for 32-byte keys in XOR ciphers (common for AES-256) by processing 256-bit blocks using four 64-bit words yields a significant (~12x) speedup compared to a generic byte-wise loop by eliminating per-byte branching and indexing overhead. Using 'memcpy' for loading and storing ensures alignment safety while still allowing the compiler to optimize the word-sized operations.
**Action:** Always provide specialized paths for cryptographic-standard key sizes (16, 32 bytes) to maximize throughput in security-critical data processing.
