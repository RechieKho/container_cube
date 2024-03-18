#ifndef CONTAINER_LIST_H
#define CONTAINER_LIST_H

#include <stdio.h>
#include <slice.h>

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
/// Return a message (`const char*`) on error, else NULL.
list_t list_reserve(list_t *m_list, size_t p_min_capacity);

/// Free all the resource in the list.
/// Return a message (`const char*`) on error, else NULL.
list_t list_clean(list_t *m_list);

/// Push an element into the list.
/// Return a message (`const char*`) on error, else NULL.
list_t list_push(list_t *m_list, data_t p_data);

/// Create a list of character (string) through formatting.
/// Return a message (`const char*`) on error, else NULL.
list_t list_string_from(list_t *r_string, const char *p_message, ...);

#endif // CONTAINER_LIST_H