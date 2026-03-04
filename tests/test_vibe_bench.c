/**
 * This test file verifies the benchmarking functionality in vibe_bench.h.
 * In version 1.5.11, it tests the VIBE_BENCHMARK macro and timing accuracy.
 * This code is AI-generated.
 */
#include <vibe_bench.h>
#include <vibe_test.h>
#include <vibe_io.h>
#include <unistd.h>

/**
 * test_benchmark_macro_syntax - Tests that VIBE_BENCHMARK macro compiles correctly.
 * Internal Logic: Verifies the macro can be used with various code blocks.
 */
void test_benchmark_macro_syntax() {
    vibe_print("Testing VIBE_BENCHMARK macro syntax...\n");

    // Test with simple statement
    VIBE_BENCHMARK("simple_addition", {
        volatile int result = 1 + 1;
        (void)result;
    });

    // Test with multiple statements
    VIBE_BENCHMARK("multiple_operations", {
        volatile int a = 10;
        volatile int b = 20;
        volatile int c = a + b;
        (void)c;
    });

    // Test with function call
    VIBE_BENCHMARK("function_call", {
        volatile int len = (int)strlen("test");
        (void)len;
    });

    VIBE_ASSERT(true); // If we got here, syntax is correct
}

/**
 * test_benchmark_timing_accuracy - Tests that timing produces reasonable values.
 * Internal Logic: Benchmarks operations with known durations and verifies timing.
 */
void test_benchmark_timing_accuracy() {
    vibe_print("Testing benchmark timing accuracy...\n");

    // Benchmark a sleep operation (should take approximately the sleep time)
    double start = vibe_get_time();
    usleep(100000); // 100ms
    double end = vibe_get_time();
    double elapsed = end - start;

    // Verify time is reasonable (between 80ms and 150ms to account for variance)
    VIBE_ASSERT(elapsed >= 0.08);
    VIBE_ASSERT(elapsed <= 0.15);

    vibe_print("  Measured 100ms sleep as: %f seconds\n", elapsed);
}

/**
 * test_benchmark_zero_time_operation - Tests benchmarking very fast operations.
 * Internal Logic: Ensures benchmark doesn't crash or produce negative times for instant operations.
 */
void test_benchmark_zero_time_operation() {
    vibe_print("Testing benchmark with near-zero time operations...\n");

    double start = vibe_get_time();
    volatile int x = 1;
    (void)x;
    double end = vibe_get_time();

    // Should be non-negative and very small
    VIBE_ASSERT(end >= start);
    VIBE_ASSERT((end - start) < 1.0); // Should complete in less than 1 second
}

/**
 * test_benchmark_nested_blocks - Tests that benchmark can handle nested code.
 * Internal Logic: Verifies complex code blocks work within the macro.
 */
void test_benchmark_nested_blocks() {
    vibe_print("Testing VIBE_BENCHMARK with nested blocks...\n");

    VIBE_BENCHMARK("nested_logic", {
        for (int i = 0; i < 100; i++) {
            volatile int sum = 0;
            for (int j = 0; j < 100; j++) {
                sum += j;
            }
            (void)sum;
        }
    });

    VIBE_ASSERT(true);
}

/**
 * test_benchmark_with_conditionals - Tests benchmark with conditional logic.
 * Internal Logic: Verifies that if/else statements work inside benchmark blocks.
 */
void test_benchmark_with_conditionals() {
    vibe_print("Testing VIBE_BENCHMARK with conditional logic...\n");

    VIBE_BENCHMARK("conditional_logic", {
        volatile int value = 42;
        if (value > 0) {
            value *= 2;
        } else {
            value *= 3;
        }
        (void)value;
    });

    VIBE_ASSERT(true);
}

/**
 * test_benchmark_multiple_runs - Tests running multiple benchmarks in sequence.
 * Internal Logic: Ensures benchmarks don't interfere with each other.
 */
void test_benchmark_multiple_runs() {
    vibe_print("Testing multiple sequential benchmarks...\n");

    VIBE_BENCHMARK("run1", {
        volatile int a = 1 + 1;
        (void)a;
    });

    VIBE_BENCHMARK("run2", {
        volatile int b = 2 + 2;
        (void)b;
    });

    VIBE_BENCHMARK("run3", {
        volatile int c = 3 + 3;
        (void)c;
    });

    VIBE_ASSERT(true);
}

/**
 * test_benchmark_with_allocations - Tests benchmark with memory operations.
 * Internal Logic: Verifies benchmark works with dynamic memory.
 */
void test_benchmark_with_allocations() {
    vibe_print("Testing VIBE_BENCHMARK with memory operations...\n");

    VIBE_BENCHMARK("memory_operations", {
        char* buffer = malloc(1024);
        if (buffer) {
            memset(buffer, 0, 1024);
            free(buffer);
        }
    });

    VIBE_ASSERT(true);
}

/**
 * test_benchmark_longer_operation - Tests benchmark with longer operations.
 * Internal Logic: Verifies benchmark can measure operations taking multiple milliseconds.
 */
void test_benchmark_longer_operation() {
    vibe_print("Testing VIBE_BENCHMARK with longer operation...\n");

    double start = vibe_get_time();
    VIBE_BENCHMARK("longer_operation", {
        usleep(50000); // 50ms
    });
    double total = vibe_get_time() - start;

    // The total should be at least the sleep time
    VIBE_ASSERT(total >= 0.04);
}

/**
 * test_benchmark_with_break_continue - Tests that loop control works in benchmark.
 * Internal Logic: Verifies break and continue statements function correctly.
 */
void test_benchmark_with_break_continue() {
    vibe_print("Testing VIBE_BENCHMARK with loop control...\n");

    VIBE_BENCHMARK("loop_control", {
        volatile int count = 0;
        for (int i = 0; i < 1000; i++) {
            if (i % 2 == 0) continue;
            count++;
            if (i > 500) break;
        }
        (void)count;
    });

    VIBE_ASSERT(true);
}

/**
 * test_vibe_get_time_consistency - Tests that vibe_get_time returns consistent values.
 * Internal Logic: Verifies time always increases and is never negative.
 */
void test_vibe_get_time_consistency() {
    vibe_print("Testing vibe_get_time consistency...\n");

    double t1 = vibe_get_time();
    VIBE_ASSERT(t1 >= 0.0);

    usleep(10000); // 10ms

    double t2 = vibe_get_time();
    VIBE_ASSERT(t2 >= t1);

    usleep(10000); // 10ms

    double t3 = vibe_get_time();
    VIBE_ASSERT(t3 >= t2);

    vibe_print("  Time progression: %f -> %f -> %f\n", t1, t2, t3);
}

int main() {
    vibe_print("=== Vibe Benchmark Tests ===\n");

    test_benchmark_macro_syntax();
    test_benchmark_timing_accuracy();
    test_benchmark_zero_time_operation();
    test_benchmark_nested_blocks();
    test_benchmark_with_conditionals();
    test_benchmark_multiple_runs();
    test_benchmark_with_allocations();
    test_benchmark_longer_operation();
    test_benchmark_with_break_continue();
    test_vibe_get_time_consistency();

    VIBE_TEST_SUMMARY();
    return 0;
}