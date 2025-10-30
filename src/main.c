#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "http_client.h"
#include "json_formatter.h"
#include "tcp_socket.h"

// Example of pointer to pointer usage: Command line argument parsing
typedef struct {
    char *host;
    char *port;
    char *path;
    double temperature;
} config_t;

/**
 * Callback function for processing HTTP response
 * Demonstrates function pointer usage
 */
void response_handler(const char *data, size_t length) {
    printf("\n=== HTTP Response Handler Called ===\n");
    printf("Response length: %zu bytes\n", length);
    printf("Data pointer: %p\n", (void*)data);
    printf("=== End Handler ===\n\n");
}

/**
 * Alternative response handler - print full response
 * Demonstrates multiple function pointer options
 */
void detailed_response_handler(const char *data, size_t length) {
    printf("\n=== Detailed HTTP Response ===\n");
    printf("Length: %zu bytes\n", length);
    printf("%s\n", data);
    printf("=== End Response ===\n\n");
}

/**
 * Parse command line arguments
 * Demonstrates char **argv (pointer to pointer) usage
 */
int parse_arguments(int argc, char **argv, config_t *config) {
    // Default configuration
    config->host = "httpbin.org";
    config->port = "80";
    config->path = "/post";
    config->temperature = 22.5;

    // Example of iterating through char **argv (pointer to pointer)
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--host") == 0 && i + 1 < argc) {
            config->host = argv[++i];
        } else if (strcmp(argv[i], "--port") == 0 && i + 1 < argc) {
            config->port = argv[++i];
        } else if (strcmp(argv[i], "--path") == 0 && i + 1 < argc) {
            config->path = argv[++i];
        } else if (strcmp(argv[i], "--temp") == 0 && i + 1 < argc) {
            config->temperature = atof(argv[++i]);
        } else if (strcmp(argv[i], "--help") == 0) {
            printf("Usage: %s [options]\n", argv[0]);
            printf("Options:\n");
            printf("  --host <hostname>    Server hostname (default: httpbin.org)\n");
            printf("  --port <port>        Server port (default: 80)\n");
            printf("  --path <path>        URL path (default: /post)\n");
            printf("  --temp <temperature> Temperature value (default: 22.5)\n");
            printf("  --help               Show this help message\n");
            return -1;
        }
    }

    return 0;
}

/**
 * Function pointer array demonstration
 * Shows how to use array of function pointers
 */
typedef void (*handler_function_t)(const char *, size_t);

void select_handler(int choice, handler_function_t *handler_ptr) {
    // Array of function pointers (demonstrates advanced pointer usage)
    static handler_function_t handlers[] = {
        response_handler,
        detailed_response_handler
    };

    if (choice >= 0 && choice < 2) {
        *handler_ptr = handlers[choice];
    } else {
        *handler_ptr = response_handler; // Default
    }
}

int main(int argc, char **argv) {
    printf("╔════════════════════════════════════════════╗\n");
    printf("║   IoT Temperature Sensor - HTTP Client     ║\n");
    printf("╚════════════════════════════════════════════╝\n\n");

    // Parse configuration from command line (demonstrates char **argv usage)
    config_t config;
    if (parse_arguments(argc, argv, &config) != 0) {
        return 0; // Help was shown
    }

    // Generate sensor data
    char uuid[64];
    char timestamp[32];
    char json_buffer[512];

    generate_uuid(uuid, sizeof(uuid));
    generate_timestamp(timestamp, sizeof(timestamp));

    printf("Device Configuration:\n");
    printf("  UUID: %s\n", uuid);
    printf("  Timestamp: %s\n", timestamp);
    printf("  Temperature: %.1f°C\n", config.temperature);
    printf("  Target: %s:%s%s\n\n", config.host, config.port, config.path);

    // Format JSON payload
    int json_len = format_sensor_json(json_buffer, sizeof(json_buffer),
                                       uuid, timestamp, config.temperature);
    
    if (json_len < 0) {
        fprintf(stderr, "Failed to format JSON\n");
        return 1;
    }

    printf("JSON Payload:\n%s\n\n", json_buffer);

    // Select response handler using function pointer
    data_handler_t handler;
    select_handler(1, &handler); // Use detailed handler

    // Send HTTP POST request
    char response_buffer[4096];
    printf("Connecting to server...\n");
    
    int result = http_post(config.host, config.port, config.path,
                           json_buffer, response_buffer, 
                           sizeof(response_buffer), handler);

    if (result == 0) {
        printf("\nSuccessfully sent temperature data!\n");
        printf("\nServer Response Preview:\n");
        printf("%.200s...\n", response_buffer);
    } else {
        fprintf(stderr, "\nError: Failed to send data\n");
        return 1;
    }

    printf("\n╔════════════════════════════════════════════╗\n");
    printf("║        Transmission Complete               ║\n");
    printf("╚════════════════════════════════════════════╝\n");

    return 0;
}
