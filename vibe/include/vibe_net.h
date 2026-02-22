/*
 * This header has been updated to include comprehensive documentation and logic explanations.
 * The changes enhance maintainability by detailing the networking API and security hardening measures.
 * THIS CODE IS AI GENERATED.
 */

#ifndef VIBE_NET_H
#define VIBE_NET_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>

/**
 * vibe_net_listen - Initializes a TCP server socket and starts listening.
 * @port: The port number to listen on
 *
 * This function creates a socket, binds it to all interfaces, and sets
 * the listen backlog to SOMAXCONN for DoS mitigation.
 * Returns the server socket file descriptor or -1 on failure.
 */
static inline int vibe_net_listen(int port) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) return -1;

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
 * @ip: The IP address of the server
 * @port: The port number of the server
 *
 * Creates a socket and attempts to connect to the specified IP and port.
 * Returns the connected socket file descriptor or -1 on failure.
 */
static inline int vibe_net_connect(const char* ip, int port) {
    if (!ip) return -1;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return -1;

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
