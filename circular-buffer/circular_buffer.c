#include "circular_buffer.h"

#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define RETURN_STATUS_DATA_FAIL                                                \
    errno = ENODATA;                                                           \
    return EXIT_FAILURE;

#define RETURN_STATUS_BUFFER_FAIL                                              \
    errno = ENOBUFS;                                                           \
    return EXIT_FAILURE;

#define RETURN_STATUS_SUCCESS                                                  \
    errno = 0;                                                                 \
    return EXIT_SUCCESS;

#define SENTINEL_VALUE_INDEX 0

struct circular_buffer_t {
    size_t capacity;
    int oldest_element_index;
    int empty_element_index;
    buffer_value_t *values;
};

static bool is_empty(struct circular_buffer_t const *const buffer) {
    return buffer->oldest_element_index == SENTINEL_VALUE_INDEX;
}

static bool is_full(struct circular_buffer_t const *const buffer) {
    return buffer->empty_element_index == buffer->oldest_element_index;
}

struct circular_buffer_t *new_circular_buffer(size_t capacity) {
    if (capacity <= 0) {
        fprintf(stderr, "Cannot create a buffer of capacity 0\n");
        return NULL;
    }

    struct circular_buffer_t *buf = malloc(sizeof(struct circular_buffer_t));

    if (buf == NULL) {
        fprintf(stderr, "Could not allocate memory for the buffer\n");
        return NULL;
    }

    buf->capacity = capacity + 1;
    buf->empty_element_index = SENTINEL_VALUE_INDEX + 1;
    buf->oldest_element_index = SENTINEL_VALUE_INDEX;
    buf->values = calloc(buf->capacity, sizeof(buffer_value_t));

    if (buf->values == NULL) {
        fprintf(stderr, "Could not allocate memory for the buffer values\n");
        free(buf);
        return NULL;
    }

    return buf;
}

void clear_buffer(struct circular_buffer_t *buffer) {
    if (buffer == NULL) {
        fprintf(stderr, "Buffer cannot be null\n");
        return;
    }

    buffer->oldest_element_index = SENTINEL_VALUE_INDEX;
    buffer->empty_element_index = SENTINEL_VALUE_INDEX + 1;
}

void delete_buffer(struct circular_buffer_t *buffer) {
    if (buffer == NULL) {
        fprintf(stderr, "Buffer cannot be null\n");
        return;
    }

    if (buffer->values != NULL) {
        free(buffer->values);
    }

    free(buffer);
    buffer = NULL;
}

int16_t read(struct circular_buffer_t *const buffer,
             buffer_value_t value[static 1]) {
    if (is_empty(buffer)) {
        RETURN_STATUS_DATA_FAIL;
    }

    *value = buffer->values[buffer->oldest_element_index];

    int next_oldest_index = buffer->oldest_element_index + 1;

    if ((size_t)next_oldest_index >= buffer->capacity) {
        next_oldest_index = SENTINEL_VALUE_INDEX + 1;
    }

    if (next_oldest_index == buffer->empty_element_index) {
        clear_buffer(buffer);
    } else {
        buffer->oldest_element_index = next_oldest_index;
    }

    RETURN_STATUS_SUCCESS;
}

int16_t write(struct circular_buffer_t *buffer, buffer_value_t value) {
    if (is_full(buffer)) {
        RETURN_STATUS_BUFFER_FAIL;
    }

    buffer->values[buffer->empty_element_index] = value;

    buffer->empty_element_index = buffer->empty_element_index + 1;

    if ((size_t)buffer->empty_element_index >= buffer->capacity) {
        buffer->empty_element_index = SENTINEL_VALUE_INDEX + 1;
    }

    if (buffer->oldest_element_index == SENTINEL_VALUE_INDEX) {
        buffer->oldest_element_index = SENTINEL_VALUE_INDEX + 1;
    }

    RETURN_STATUS_SUCCESS;
}

int16_t overwrite(struct circular_buffer_t *buffer, buffer_value_t value) {
    if (!is_full(buffer)) {
        return write(buffer, value);
    }

    buffer->values[buffer->oldest_element_index] = value;

    buffer->oldest_element_index = buffer->oldest_element_index + 1;

    if ((size_t)buffer->oldest_element_index >= buffer->capacity) {
        buffer->oldest_element_index = SENTINEL_VALUE_INDEX + 1;
    }

    buffer->empty_element_index = buffer->oldest_element_index;

    RETURN_STATUS_SUCCESS;
}
