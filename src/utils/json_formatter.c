#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "json_formatter.h"

char *generate_timestamp(char *buffer, size_t buffer_size) {
    if (!buffer || buffer_size < 20) {
        return NULL;
    }

    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    
    if (!tm_info) {
        fprintf(stderr, "Error getting local time\n");
        return NULL;
    }

    // Format: YYYY-MM-DD HH:MM:SS
    strftime(buffer, buffer_size, "%Y-%m-%d %H:%M:%S", tm_info);
    
    return buffer;
}

char *generate_uuid(char *buffer, size_t buffer_size) {
    if (!buffer || buffer_size < 37) {
        return NULL;
    }

    // Simple UUID generation (not truly unique, for demonstration)
    // Format: xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx
    srand(time(NULL));
    
    snprintf(buffer, buffer_size,
             "%08x-%04x-4%03x-%04x-%012lx",
             rand(),
             rand() & 0xFFFF,
             rand() & 0xFFF,
             (rand() & 0x3FFF) | 0x8000,
             (long)(rand() % 1000000000000L));
    
    return buffer;
}

int format_sensor_json(char *buffer, size_t buffer_size, 
                       const char *device_id, 
                       const char *timestamp, 
                       double temperature) {
    if (!buffer || !device_id || !timestamp) {
        fprintf(stderr, "Error: Invalid parameters for format_sensor_json\n");
        return -1;
    }

    // Build JSON string
    int written = snprintf(buffer, buffer_size,
        "{\n"
        "  \"device\": \"%s\",\n"
        "  \"time\": \"%s\",\n"
        "  \"temperature\": \"%.1f°C\"\n"
        "}",
        device_id, timestamp, temperature);

    if (written < 0 || (size_t)written >= buffer_size) {
        fprintf(stderr, "Error: Buffer too small for JSON formatting\n");
        return -1;
    }

    return written;
}
