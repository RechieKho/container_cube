#ifndef CONTAINER_ALLOC_H
#define CONTAINER_ALLOC_H

#include <stdlib.h>

/// Shorthand allocation.
#define ALLOC(mp_ptr, mp_size) \
    realloc((mp_ptr), (mp_size))

/// Shorthand typed allocation.
#define ALLOC_TYPED(id_type) \
    (id_type *)calloc(1, sizeof(id_type))

/// Shorthand free.
#define FREE(mp_allocated) \
    free((void *)mp_allocated)

#endif // CONTAINER_ALLOCA_H