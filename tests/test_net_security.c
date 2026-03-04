/**
 * This test verifies the security hardening of the networking utilities.
 * In version 1.5.11, it checks for port validation, SO_REUSEADDR, and FD_CLOEXEC flags.
 * This code is AI-generated.
 */
#include <vibe_net.h>
#include <vibe_io.h>
#include <vibe_test.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <errno.h>

/**
 * Verify that networking functions reject out-of-range port numbers.
 *
 * Asserts that attempting to listen or connect using ports less than 0 or greater than 65535 fails.
 */
void test_port_validation() {
    vibe_print("Testing port validation...\n");
    VIBE_ASSERT(vibe_net_listen(-1) == -1);
    VIBE_ASSERT(vibe_net_listen(65536) == -1);
    VIBE_ASSERT(vibe_net_connect("127.0.0.1", -1) == -1);
    VIBE_ASSERT(vibe_net_connect("127.0.0.1", 65536) == -1);
}

/**
 * Verify that a newly created listening socket has SO_REUSEADDR enabled and FD_CLOEXEC set.
 *
 * If socket creation fails the function logs the failure and returns without performing checks.
 */
void test_socket_options() {
    vibe_print("Testing socket options...\n");
    int port = 9999;
    int fd = vibe_net_listen(port);
    if (fd < 0) {
        vibe_print("Failed to listen on port %d: %d\n", port, errno);
        return;
    }

    // Check SO_REUSEADDR
    int optval;
    socklen_t optlen = sizeof(optval);
    VIBE_ASSERT(getsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, &optlen) == 0);
    VIBE_ASSERT(optval != 0);

    // Check FD_CLOEXEC
    int flags = fcntl(fd, F_GETFD);
    VIBE_ASSERT(flags != -1);
    VIBE_ASSERT(flags & FD_CLOEXEC);

    close(fd);
}

/**
 * Execute network security tests (port validation and socket options) and print a test summary.
 *
 * Runs test_port_validation() and test_socket_options(), then emits the aggregated test
 * results via VIBE_TEST_SUMMARY() before exiting.
 *
 * @returns 0 on successful completion of the test program.
 */
int main() {
    test_port_validation();
    test_socket_options();
    VIBE_TEST_SUMMARY();
    return 0;
}
