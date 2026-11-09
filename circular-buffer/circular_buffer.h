#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef int buffer_value_t;

struct circular_buffer_t *new_circular_buffer(size_t capacity);
void clear_buffer(struct circular_buffer_t *buffer);
void delete_buffer(struct circular_buffer_t *buffer);

int16_t read(struct circular_buffer_t *buffer, buffer_value_t value[static 1]);
int16_t write(struct circular_buffer_t *buffer, buffer_value_t value);
int16_t overwrite(struct circular_buffer_t *buffer, buffer_value_t value);

#endif
