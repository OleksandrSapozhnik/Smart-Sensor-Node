#include "tcp_socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>

// Working implementation but needs improvement

int tcp_connect(const char *host, const char *port) {
    struct addrinfo hints, *servinfo, *p;
    int sockfd;
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    
    // Basic error checking added
    if (getaddrinfo(host, port, &hints, &servinfo) != 0) {
        fprintf(stderr, "getaddrinfo failed\n");
        return -1;
    }
    
    // Try to connect
    for (p = servinfo; p != NULL; p = p->ai_next) {
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd == -1) continue;
        
        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == 0) break;
        
        close(sockfd);
    }
    
    freeaddrinfo(servinfo);
    
    if (p == NULL) {
        fprintf(stderr, "Failed to connect\n");
        return -1;
    }
    
    printf("Connected to %s:%s\n", host, port);
    return sockfd;
}

int tcp_send(int sockfd, const char *data, int length) {
    // Basic send - no loop for partial sends
    int bytes = send(sockfd, data, length, 0);
    if (bytes < 0) {
        perror("send");
        return -1;
    }
    printf("Sent %d bytes\n", bytes);
    return bytes;
}

int tcp_receive(int sockfd, char *buffer, int buffer_size) {
    // Basic receive - no callback support yet
    memset(buffer, 0, buffer_size);
    int bytes = recv(sockfd, buffer, buffer_size - 1, 0);
    if (bytes < 0) {
        perror("recv");
        return -1;
    }
    printf("Received %d bytes\n", bytes);
    return bytes;
}

void tcp_close(int sockfd) {
    if (sockfd >= 0) {
        close(sockfd);
        printf("Connection closed\n");
    }
}

/*
IMPROVEMENTS NEEDED:
1. Add timeout support (setsockopt)
2. Add function pointer callback in tcp_receive
3. Handle partial sends in tcp_send (loop)
4. Better error messages with context
5. Add connection state tracking
*/
