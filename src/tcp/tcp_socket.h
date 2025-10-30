#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H

#define _POSIX_C_SOURCE 200112L 
#define TIMEOUT_SECONDS 10

#include <stddef.h>

/**
 * TCP Socket Module
 * Handles TCP connection and data transmission
 */

// Function pointer type for data processing callbacks
typedef void (*data_handler_t)(const char *data, size_t length);

/**
 * Create and connect a TCP socket to specified host and port
 * @param host Hostname or IP address
 * @param port Port number
 * @return Socket file descriptor or -1 on error
 */
int tcp_connect(const char *host, const char *port);

/**
 * Send data through TCP socket
 * @param sockfd Socket file descriptor
 * @param data Data buffer to send
 * @param length Length of data
 * @return Number of bytes sent or -1 on error
 */
int tcp_send(int sockfd, const char *data, size_t length);

/**
 * Receive data from TCP socket
 * @param sockfd Socket file descriptor
 * @param buffer Buffer to store received data
 * @param buffer_size Size of buffer
 * @param handler Optional callback function to process data
 * @return Number of bytes received or -1 on error
 */
int tcp_receive(int sockfd, char *buffer, size_t buffer_size, data_handler_t handler);

/**
 * Close TCP socket connection
 * @param sockfd Socket file descriptor
 */
void tcp_close(int sockfd);

#endif // TCP_SOCKET_H
