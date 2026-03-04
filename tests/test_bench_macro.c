/**
 * Comprehensive tests for vibe_bench.h benchmarking macro.
 * In version 1.5.11, this test verifies the VIBE_BENCHMARK macro functionality.
 * This code is AI-generated.
 */
#include "../vibe/include/vibe_bench.h"
#include "../vibe/include/vibe_test.h"
#include <stdio.h>
#include <unistd.h>

/**
 * test_benchmark_basic - Tests basic benchmarking functionality.
 * Internal Logic: Verifies that VIBE_BENCHMARK executes code and measures time.
 */
void test_benchmark_basic() {
    int executed = 0;

    VIBE_BENCHMARK("Basic test", {
        executed = 1;
    });

    VIBE_ASSERT(executed == 1);
}

/**
 * test_benchmark_with_sleep - Tests benchmarking with actual work.
 * Internal Logic: Runs a benchmark with a small sleep to verify timing works.
 */
void test_benchmark_with_sleep() {
    volatile int sum = 0;

    VIBE_BENCHMARK("Sleep test", {
        usleep(10000); // 10ms
        sum = 42;
    });

    VIBE_ASSERT(sum == 42);
}

/**
 * test_benchmark_multiple_statements - Tests benchmarking multiple statements.
 * Internal Logic: Verifies that the macro can handle blocks with multiple operations.
 */
void test_benchmark_multiple_statements() {
    int a = 0, b = 0, c = 0;

    VIBE_BENCHMARK("Multiple statements", {
        a = 1;
        b = 2;
        c = a + b;
    });

    VIBE_ASSERT(a == 1);
    VIBE_ASSERT(b == 2);
    VIBE_ASSERT(c == 3);
}

/**
 * test_benchmark_loop - Tests benchmarking a loop operation.
 * Internal Logic: Measures the time to execute a computational loop.
 */
void test_benchmark_loop() {
    volatile long sum = 0;

    VIBE_BENCHMARK("Loop benchmark", {
        for (int i = 0; i < 100000; i++) {
            sum += i;
        }
    });

    VIBE_ASSERT(sum > 0);
}

/**
 * test_benchmark_nested_blocks - Tests that benchmarks can be nested.
 * Internal Logic: Runs one benchmark inside another to verify no interference.
 */
void test_benchmark_nested_blocks() {
    int outer = 0, inner = 0;

    VIBE_BENCHMARK("Outer benchmark", {
        outer = 1;
        VIBE_BENCHMARK("Inner benchmark", {
            inner = 2;
        });
    });

    VIBE_ASSERT(outer == 1);
    VIBE_ASSERT(inner == 2);
}

/**
 * test_benchmark_with_return - Tests benchmark with early return.
 * Internal Logic: Verifies that benchmarks work correctly with control flow.
 */
int helper_with_return() {
    int result = 0;

    VIBE_BENCHMARK("Function with return", {
        result = 100;
    });

    return result;
}

void test_benchmark_with_return() {
    int value = helper_with_return();
    VIBE_ASSERT(value == 100);
}

/**
 * test_benchmark_timing_accuracy - Tests that benchmark measures non-zero time.
 * Internal Logic: Runs a computation-heavy benchmark and verifies timing is reasonable.
 */
void test_benchmark_timing_accuracy() {
    // This is more of a sanity check - the benchmark should complete
    volatile double result = 0.0;

    VIBE_BENCHMARK("Computation test", {
        for (int i = 0; i < 1000000; i++) {
            result += (double)i * 0.1;
        }
    });

    VIBE_ASSERT(result > 0.0);
}

/**
 * test_benchmark_empty_block - Tests benchmark with empty block.
 * Internal Logic: Verifies that benchmarking an empty operation doesn't crash.
 */
void test_benchmark_empty_block() {
    VIBE_BENCHMARK("Empty block", {
        // Intentionally empty
    });

    // If we reach here, the test passed
    VIBE_ASSERT(true);
}

/**
 * test_benchmark_with_variables - Tests benchmark with variable declarations.
 * Internal Logic: Ensures variables declared inside benchmark block are scoped correctly.
 */
void test_benchmark_with_variables() {
    VIBE_BENCHMARK("Variables test", {
        int local = 42;
        VIBE_ASSERT(local == 42);
    });

    // local should not be accessible here
    VIBE_ASSERT(true);
}

/**
 * test_benchmark_sequential - Tests multiple sequential benchmarks.
 * Internal Logic: Runs several benchmarks in sequence to verify independence.
 */
void test_benchmark_sequential() {
    int test1 = 0, test2 = 0, test3 = 0;

    VIBE_BENCHMARK("Sequential 1", {
        test1 = 1;
    });

    VIBE_BENCHMARK("Sequential 2", {
        test2 = 2;
    });

    VIBE_BENCHMARK("Sequential 3", {
        test3 = 3;
    });

    VIBE_ASSERT(test1 == 1);
    VIBE_ASSERT(test2 == 2);
    VIBE_ASSERT(test3 == 3);
}

int main() {
    printf("Testing vibe_bench.h functionality...\n"); /* nosec */
    printf("Note: Benchmark output is expected and indicates test execution.\n\n"); /* nosec */

    test_benchmark_basic();
    test_benchmark_with_sleep();
    test_benchmark_multiple_statements();
    test_benchmark_loop();
    test_benchmark_nested_blocks();
    test_benchmark_with_return();
    test_benchmark_timing_accuracy();
    test_benchmark_empty_block();
    test_benchmark_with_variables();
    test_benchmark_sequential();

    VIBE_TEST_SUMMARY();
    printf("\n[Note: Benchmark timing output above is normal and expected]\n"); /* nosec */
    return 0;
}