#include "tcp_socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>
#include <sys/time.h>


int tcp_connect(const char *host, const char *port) {
    if (!host || !port) {
        fprintf(stderr, "Error: Invalid host or port\n");
        return -1;
    }

    struct addrinfo hints, *servinfo, *p;
    int sockfd;
    int rv;

    // Configure address info hints
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;      // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP
    
    // Get address info for host
    if ((rv = getaddrinfo(host, port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(rv));
        return -1;
    }

    // Loop through results and connect to first successful
    for (p = servinfo; p != NULL; p = p->ai_next) {
        // Create socket
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("socket");
            continue;
        }

        // Set socket timeout
        struct timeval timeout;
        timeout.tv_sec = TIMEOUT_SECONDS;
        timeout.tv_usec = 0;
        
        if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
            perror("setsockopt (receive timeout)");
        }
        
        if (setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) < 0) {
            perror("setsockopt (send timeout)");
        }

        // Connect to server
        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("connect");
            continue;
        }

        break; // Successfully connected
    }

    freeaddrinfo(servinfo);

    if (p == NULL) {
        fprintf(stderr, "Failed to connect to %s:%s\n", host, port);
        return -1;
    }

    printf("Successfully connected to %s:%s\n", host, port);
    return sockfd;
}

int tcp_send(int sockfd, const char *data, size_t length) {
    if (sockfd < 0 || !data || length == 0) {
        fprintf(stderr, "Error: Invalid parameters for tcp_send\n");
        return -1;
    }

    ssize_t total_sent = 0;
    ssize_t bytes_sent;

    // Send all data (may require multiple calls)
    while (total_sent < (ssize_t)length) {
        bytes_sent = send(sockfd, data + total_sent, length - total_sent, 0);
        
        if (bytes_sent == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // Timeout occurred
                fprintf(stderr, "Send timeout\n");
            } else {
                perror("send");
            }
            return -1;
        }
        
        total_sent += bytes_sent;
    }

    printf("Sent %zd bytes\n", total_sent);
    return (int)total_sent;
}

int tcp_receive(int sockfd, char *buffer, size_t buffer_size, data_handler_t handler) {
    if (sockfd < 0 || !buffer || buffer_size == 0) {
        fprintf(stderr, "Error: Invalid parameters for tcp_receive\n");
        return -1;
    }

    // Clear buffer
    memset(buffer, 0, buffer_size);

    ssize_t bytes_received = recv(sockfd, buffer, buffer_size - 1, 0);
    
    if (bytes_received == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            fprintf(stderr, "Receive timeout\n");
        } else {
            perror("recv");
        }
        return -1;
    } else if (bytes_received == 0) {
        printf("Connection closed by server\n");
        return 0;
    }

    buffer[bytes_received] = '\0'; // Null-terminate

    printf("Received %zd bytes\n", bytes_received);

    // Call handler if provided (demonstration of function pointer)
    if (handler) {
        handler(buffer, bytes_received);
    }

    return (int)bytes_received;
}

void tcp_close(int sockfd) {
    if (sockfd >= 0) {
        close(sockfd);
        printf("Socket closed\n");
    }
}
