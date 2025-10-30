#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H

#define _POSIX_C_SOURCE 200112L

// Working but needs improvement
// Added: Basic modularity, some error handling
// Missing: Function pointers, advanced features

int tcp_connect(const char *host, const char *port);
int tcp_send(int sockfd, const char *data, int length);
int tcp_receive(int sockfd, char *buffer, int buffer_size);
void tcp_close(int sockfd);

// TODO: Add function pointer for callbacks
// TODO: Add timeout handling
// TODO: Better error reporting

#endif
