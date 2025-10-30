#include "json_formatter.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

// Basic JSON formatting, needs improvement

char *generate_timestamp(char *buffer, int buffer_size) {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    
    // Simple format - ISO 8601
    strftime(buffer, buffer_size, "%Y-%m-%d %H:%M:%S", tm_info);
    return buffer;
}

int format_sensor_json(char *buffer, int buffer_size, 
                       const char *device_id, 
                       const char *timestamp, 
                       double temperature) {
    
    // Basic JSON - no error checking on buffer size
    int len = sprintf(buffer,
        "{\n"
        "  \"device\": \"%s\",\n"
        "  \"time\": \"%s\",\n"
        "  \"temperature\": \"%.1f°C\"\n"
        "}",
        device_id, timestamp, temperature);
    
    // TODO: Check if buffer is big enough
    // TODO: Escape special characters in strings
    
    return len;
}

char *generate_uuid(char *buffer, int buffer_size) {
    // Very simple UUID - not cryptographically secure
    srand(time(NULL));
    sprintf(buffer, "%08x-%04x-%04x-%04x-%012x",
            rand(), rand() & 0xFFFF, rand() & 0xFFFF,
            rand() & 0xFFFF, rand());
    return buffer;
}

/*
IMPROVEMENTS NEEDED:
1. Use snprintf instead of sprintf for safety
2. Validate buffer sizes before writing
3. Better UUID generation (more random)
4. Add JSON escaping for special characters
5. Add error return codes
*/
