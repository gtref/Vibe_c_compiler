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
 * test_port_validation - Verifies that invalid ports are correctly rejected.
 * Internal Logic: Attempts to listen and connect with out-of-range port numbers.
 */
void test_port_validation() {
    vibe_print("Testing port validation...\n");
    VIBE_ASSERT(vibe_net_listen(-1) == -1);
    VIBE_ASSERT(vibe_net_listen(65536) == -1);
    VIBE_ASSERT(vibe_net_connect("127.0.0.1", -1) == -1);
    VIBE_ASSERT(vibe_net_connect("127.0.0.1", 65536) == -1);
}

/**
 * test_socket_options - Verifies that security-critical socket options are applied.
 * Internal Logic: Checks for SO_REUSEADDR and FD_CLOEXEC on a successfully created socket.
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

int main() {
    test_port_validation();
    test_socket_options();
    VIBE_TEST_SUMMARY();
    return 0;
}
