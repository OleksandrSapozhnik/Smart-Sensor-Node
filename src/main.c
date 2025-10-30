#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tcp/tcp_socket.h"
#include "utils/json_formatter.h"

// Working but needs improvement
// Missing: Function pointers, advanced callback system

typedef struct {
    char *host;
    char *port;
    double temperature;
} config_t;

// Basic argument parsing - demonstrates char **argv
int parse_arguments(int argc, char **argv, config_t *config) {
    // Default values
    config->host = "httpbin.org";
    config->port = "80";
    config->temperature = 22.5;
    
    // Simple parsing - no validation
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--host") == 0 && i + 1 < argc) {
            config->host = argv[++i];
        } else if (strcmp(argv[i], "--port") == 0 && i + 1 < argc) {
            config->port = argv[++i];
        } else if (strcmp(argv[i], "--temp") == 0 && i + 1 < argc) {
            config->temperature = atof(argv[++i]);
        }
    }
    
    return 0;
}

int main(int argc, char **argv) {
    printf("IoT Temperature Client - Version 2 (Working)\n\n");
    
    // Parse command line arguments
    config_t config;
    parse_arguments(argc, argv, &config);
    
    // Generate sensor data
    char uuid[64];
    char timestamp[32];
    char json_buffer[512];
    
    generate_uuid(uuid, sizeof(uuid));
    generate_timestamp(timestamp, sizeof(timestamp));
    
    printf("Configuration:\n");
    printf("  Device: %s\n", uuid);
    printf("  Time: %s\n", timestamp);
    printf("  Temperature: %.1f°C\n", config.temperature);
    printf("  Target: %s:%s\n\n", config.host, config.port);
    
    // Format JSON
    format_sensor_json(json_buffer, sizeof(json_buffer),
                      uuid, timestamp, config.temperature);
    
    printf("JSON Data:\n%s\n\n", json_buffer);
    
    // Connect via TCP
    int sockfd = tcp_connect(config.host, config.port);
    if (sockfd < 0) {
        fprintf(stderr, "Connection failed\n");
        return 1;
    }
    
    // Build HTTP POST request
    char http_request[2048];
    sprintf(http_request,
        "POST /post HTTP/1.1\r\n"
        "Host: %s\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: %ld\r\n"
        "\r\n"
        "%s",
        config.host, strlen(json_buffer), json_buffer);
    
    printf("Sending HTTP request...\n");
    if (tcp_send(sockfd, http_request, strlen(http_request)) < 0) {
        tcp_close(sockfd);
        return 1;
    }
    
    // Receive response
    char response[4096];
    if (tcp_receive(sockfd, response, sizeof(response)) < 0) {
        tcp_close(sockfd);
        return 1;
    }
    
    printf("\nResponse preview:\n%.200s...\n", response);
    
    tcp_close(sockfd);
    
    printf("\n✓ Data sent successfully!\n");
    return 0;
}

/*
WHAT WORKS:
✓ Command line argument parsing (char **argv)
✓ Modular code structure
✓ Basic error handling
✓ TCP connection
✓ HTTP POST
✓ JSON formatting

WHAT'S MISSING FOR FULL REQUIREMENTS:
✗ Function pointers (no callbacks yet)
✗ Array of function pointers
✗ Advanced pointer usage examples
✗ Timeout handling
✗ HTTP client as separate module
✗ Better documentation

NEXT STEPS (for v2):
1. Add function pointer callbacks
2. Create http_client module
3. Add multiple handler functions
4. Improve error handling with timeouts
5. Better code documentation
*/
