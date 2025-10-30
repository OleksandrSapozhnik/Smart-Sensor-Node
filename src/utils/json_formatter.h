#ifndef JSON_FORMATTER_H
#define JSON_FORMATTER_H

// Working but basic

char *generate_timestamp(char *buffer, int buffer_size);
char *generate_uuid(char *buffer, int buffer_size);
int format_sensor_json(char *buffer, int buffer_size, 
                       const char *device_id, 
                       const char *timestamp, 
                       double temperature);

// TODO: Add size_t for sizes (better practice)
// TODO: Add const for read-only parameters

#endif
