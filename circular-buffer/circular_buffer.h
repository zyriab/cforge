#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <stdbool.h>
#include <stdint.h>

typedef int buffer_value_t;
typedef struct circular_buffer_t circular_buffer_t;
typedef circular_buffer_t *cbuf_handle_t;

cbuf_handle_t new_circular_buffer(int capacity);
void clear_buffer(cbuf_handle_t buffer);
void delete_buffer(cbuf_handle_t buffer);

int16_t read(const cbuf_handle_t buffer, buffer_value_t value[static 1]);
int16_t write(cbuf_handle_t buffer, buffer_value_t value);
int16_t overwrite(cbuf_handle_t buffer, buffer_value_t value);

#endif
