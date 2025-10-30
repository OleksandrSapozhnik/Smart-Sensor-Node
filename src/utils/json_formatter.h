#ifndef JSON_FORMATTER_H
#define JSON_FORMATTER_H

#include <stddef.h>

/**
 * JSON Formatter Module
 * Creates JSON formatted data for IoT sensor readings
 */

/**
 * Generate current timestamp in ISO 8601 format
 * @param buffer Buffer to store timestamp
 * @param buffer_size Size of buffer
 * @return Pointer to buffer or NULL on error
 */
char *generate_timestamp(char *buffer, size_t buffer_size);

/**
 * Format sensor data as JSON
 * @param buffer Buffer to store JSON string
 * @param buffer_size Size of buffer
 * @param device_id UUID or device identifier
 * @param timestamp Timestamp string
 * @param temperature Temperature value
 * @return Length of generated JSON string or -1 on error
 */
int format_sensor_json(char *buffer, size_t buffer_size, 
                       const char *device_id, 
                       const char *timestamp, 
                       double temperature);

/**
 * Generate a simple UUID (for demonstration purposes)
 * @param buffer Buffer to store UUID
 * @param buffer_size Size of buffer
 * @return Pointer to buffer or NULL on error
 */
char *generate_uuid(char *buffer, size_t buffer_size);

#endif // JSON_FORMATTER_H
