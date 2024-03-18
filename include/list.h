#ifndef CONTAINER_LIST_H
#define CONTAINER_LIST_H

#include <stdio.h>
#include <slice.h>
#include <error.h>

typedef struct list_t
{
    slice_t slice;
    size_t capacity;
} list_t;

#define LIST(mp_size)               \
    (list_t)                        \
    {                               \
        .slice = (slice_t){         \
            .data = (data_t){       \
                .ptr = NULL,        \
                .size = (mp_size)}, \
            .length = 0},           \
        .capacity = 0               \
    }

/// Reallocate memory with the capacity to be at least `p_min_capacity`.
error_t list_reserve(list_t *m_list, size_t p_min_capacity);

/// Free all the resource in the list.
error_t list_clean(list_t *m_list);

/// Push an element into the list.
error_t list_push(list_t *m_list, data_t p_data);

/// Append formatted string into a list of characters.
error_t list_string_append(list_t *m_string, const char *p_message, ...);

#endif // CONTAINER_LIST_H