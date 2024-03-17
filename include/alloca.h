#ifndef CONTAINER_ALLOCA_H
#define CONTAINER_ALLOCA_H

#include <stdlib.h>

/// Shorthand allocation.
#define ALLOC(id_type) \
    (id_type *)calloc(1, sizeof(id_type))

/// Shorthand free.
#define FREE(mp_allocated) \
    free((void *)mp_allocated)

#endif // CONTAINER_ALLOCA_H