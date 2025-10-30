#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "http_client.h"

int build_http_post_request(char *buffer, size_t buffer_size,
                            const char *host, const char *path,
                            const char *body) {
    if (!buffer || !host || !path || !body) {
        fprintf(stderr, "Error: Invalid parameters for build_http_post_request\n");
        return -1;
    }

    size_t content_length = strlen(body);
    
    int written = snprintf(buffer, buffer_size,
        "POST %s HTTP/1.1\r\n"
        "Host: %s\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "\r\n"
        "%s",
        path, host, content_length, body);

    if (written < 0 || (size_t)written >= buffer_size) {
        fprintf(stderr, "Error: Buffer too small for HTTP request\n");
        return -1;
    }

    return written;
}

int http_post(const char *host, const char *port, const char *path,
              const char *json_body, char *response_buffer, 
              size_t buffer_size, data_handler_t handler) {
    
    if (!host || !port || !path || !json_body || !response_buffer) {
        fprintf(stderr, "Error: Invalid parameters for http_post\n");
        return -1;
    }

    // Connect to server
    int sockfd = tcp_connect(host, port);
    if (sockfd < 0) {
        fprintf(stderr, "Failed to connect to server\n");
        return -1;
    }

    // Build HTTP POST request
    char request_buffer[4096];
    int request_length = build_http_post_request(request_buffer, sizeof(request_buffer),
                                                  host, path, json_body);
    
    if (request_length < 0) {
        tcp_close(sockfd);
        return -1;
    }

    printf("\n--- Sending HTTP POST Request ---\n");
    printf("%s\n", request_buffer);
    printf("--- End of Request ---\n\n");

    // Send request
    if (tcp_send(sockfd, request_buffer, request_length) < 0) {
        fprintf(stderr, "Failed to send HTTP request\n");
        tcp_close(sockfd);
        return -1;
    }

    // Receive response
    int bytes_received = tcp_receive(sockfd, response_buffer, buffer_size, handler);
    
    if (bytes_received < 0) {
        fprintf(stderr, "Failed to receive HTTP response\n");
        tcp_close(sockfd);
        return -1;
    }

    // Close connection
    tcp_close(sockfd);

    return 0;
}
