/**
 * This header provides networking utilities for TCP listening and connecting in the Vibe C library.
 * In version 1.5.11, it maintains robust networking with port validation, SO_REUSEADDR for immediate restarts, and FD_CLOEXEC flags for security.
 * This code is AI-generated.
 */
#ifndef VIBE_NET_H
#define VIBE_NET_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

/**
 * vibe_net_listen - Creates a TCP server socket listening on the specified port.
 * Internal Logic: Binds to INADDR_ANY and uses SOMAXCONN for the listen backlog to mitigate DoS.
 */
static inline int vibe_net_listen(int port) {
    // Internal Logic: Validate port range to ensure it's within standard TCP bounds.
    if (port < 0 || port > 65535) return -1;

    // Internal Logic: Open a new TCP socket using the AF_INET domain.
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) return -1;

    // Internal Logic: Set SO_REUSEADDR to allow immediate restart of the server on the same port.
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        close(server_fd);
        return -1;
    }

    // Internal Logic: Set FD_CLOEXEC to prevent the socket from being inherited by child processes.
    int flags = fcntl(server_fd, F_GETFD);
    if (flags == -1 || fcntl(server_fd, F_SETFD, flags | FD_CLOEXEC) == -1) {
        close(server_fd);
        return -1;
    }

    // Internal Logic: Zero-initialize sockaddr_in to prevent leaking uninitialized stack data to the kernel.
    struct sockaddr_in address = {0};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Internal Logic: Bind the socket to the specified port and start listening with a hardened backlog.
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        close(server_fd);
        return -1;
    }

    if (listen(server_fd, SOMAXCONN) < 0) {
        close(server_fd);
        return -1;
    }

    return server_fd;
}

/**
 * vibe_net_connect - Connects to a remote TCP server.
 * Internal Logic: Performs DNS-less connection using inet_pton and standard POSIX connect.
 */
static inline int vibe_net_connect(const char* ip, int port) {
    // Internal Logic: Validate IP and port inputs before initializing the socket for connection.
    if (!ip || port < 0 || port > 65535) return -1;

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return -1;

    // Internal Logic: Set FD_CLOEXEC on the connection socket to improve security in multi-process environments.
    int flags = fcntl(sock, F_GETFD);
    if (flags == -1 || fcntl(sock, F_SETFD, flags | FD_CLOEXEC) == -1) {
        close(sock);
        return -1;
    }

    // Internal Logic: Zero-initialize the server address structure to maintain security and consistency.
    struct sockaddr_in serv_addr = {0};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // Internal Logic: Convert the string IP address to binary format and attempt to connect.
    if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) {
        close(sock);
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        close(sock);
        return -1;
    }

    return sock;
}

/**
 * vibe_net_accept - Accepts a new connection on a server socket.
 * Internal Logic: Wraps the standard accept call and applies FD_CLOEXEC for security.
 */
static inline int vibe_net_accept(int server_fd, struct sockaddr *addr, socklen_t *addrlen) {
    // Internal Logic: Perform the standard accept operation.
    int client_fd = accept(server_fd, addr, addrlen);
    if (client_fd < 0) return -1;

    // Internal Logic: Set FD_CLOEXEC on the new client socket to prevent descriptor leakage.
    int flags = fcntl(client_fd, F_GETFD);
    if (flags == -1 || fcntl(client_fd, F_SETFD, flags | FD_CLOEXEC) == -1) {
        close(client_fd);
        return -1;
    }

    return client_fd;
}

#endif
