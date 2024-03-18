#ifndef CONTAINER_DATA_H
#define CONTAINER_DATA_H

#include <stddef.h>

/// A piece of data.
typedef struct data_t
{
    void *ptr;
    size_t size;
} data_t;

/// Shorthand literal.
#define DATA(mp_ptr, mp_size) \
    (data_t) { .ptr = (mp_ptr), .size = (mp_size) }

/// Cast the `mp_data.ptr` pointer to given `id_type` type.
/// Results `id_ref` variable that points to `mp_data.ptr` (a double pointer).
/// `id_ref` is `NULL` when the size is not the same.
#define DATA_CAST(id_type, id_ref, mp_data) \
    id_type **const id_ref = sizeof(id_type) == (mp_data).size ? (id_type **)&(mp_data).ptr : NULL;

#endif // CONTAINER_DATA_H