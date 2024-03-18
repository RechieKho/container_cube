#ifndef CONTAINER_COW_H
#define CONTAINER_COW_H

#include <stdbool.h>
#include <data.h>

/// Copy-On-Write data.
typedef struct cow_t
{
    bool borrowed;
    data_t data;
} cow_t;

/// Shorthanded literal.
#define COW(mp_borrowed, mp_ptr, mp_size) \
    (cow_t)                               \
    {                                     \
        .borrowed = (mp_borrowed),        \
        .data = (data_t)                  \
        {                                 \
            .ptr = (mp_ptr),              \
            .size = (mp_size)             \
        }                                 \
    }

/// Set `mp_status` to `mp_message` (a c-string error message) and go to `id_label`,
/// when `mp_condition` evaluate to true.
/// It is used to raise error.
#define RAISE(mp_status, id_label, mp_condition, mp_message) \
    if (mp_condition)                                        \
    {                                                        \
        (mp_status) = COW(false, mp_message, sizeof(char));  \
        goto id_label;                                       \
    }

/// Set result of `mp_expression` to `mp_status`.
/// Go to `id_label` if `mp_status` having valid data.
/// It is used to catch and/or rethrow error.
#define TRY(mp_status, id_label, mp_expression)           \
    if (((mp_status) = (mp_expression)).data.ptr != NULL) \
    goto id_label

#endif // CONTAINER_COW_H