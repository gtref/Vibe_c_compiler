/**
 * This header provides networking utilities for TCP listening and connecting in the Vibe C library.
 * In version 1.5.8, it continues to provide hardened socket structures and SOMAXCONN backlogs.
 * This code is AI-generated.
 */
#ifndef VIBE_NET_H
#define VIBE_NET_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>

/**
 * vibe_net_listen - Creates a TCP server socket listening on the specified port.
 * Internal Logic: Binds to INADDR_ANY and uses SOMAXCONN for the listen backlog to mitigate DoS.
 */
static inline int vibe_net_listen(int port) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) return -1;

    // Internal Logic: Zero-initialize sockaddr_in to prevent information leakage from stack data.
    struct sockaddr_in address = {0};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

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
    if (!ip) return -1;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return -1;

    // Internal Logic: Zero-initialize sockaddr_in to prevent information leakage.
    struct sockaddr_in serv_addr = {0};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

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

#endif
