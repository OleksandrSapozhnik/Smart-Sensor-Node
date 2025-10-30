#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <stddef.h>
#include "tcp_socket.h"

/**
 * HTTP Client Module
 * Handles HTTP POST requests over TCP
 */

// HTTP response structure
typedef struct {
    int status_code;
    char *headers;
    char *body;
    size_t body_length;
} http_response_t;

/**
 * Send HTTP POST request
 * @param host Hostname (e.g., "httpbin.org")
 * @param port Port number (e.g., "80")
 * @param path URL path (e.g., "/post")
 * @param json_body JSON data to send in POST body
 * @param response_buffer Buffer to store response
 * @param buffer_size Size of response buffer
 * @param handler Optional callback for processing response
 * @return 0 on success, -1 on error
 */
int http_post(const char *host, const char *port, const char *path,
              const char *json_body, char *response_buffer, 
              size_t buffer_size, data_handler_t handler);

/**
 * Build HTTP POST request string
 * @param buffer Buffer to store HTTP request
 * @param buffer_size Size of buffer
 * @param host Hostname for Host header
 * @param path URL path
 * @param body Request body (JSON)
 * @return Length of generated request or -1 on error
 */
int build_http_post_request(char *buffer, size_t buffer_size,
                            const char *host, const char *path,
                            const char *body);

#endif // HTTP_CLIENT_H
