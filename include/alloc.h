#ifndef CONTAINER_ALLOC_H
#define CONTAINER_ALLOC_H

#include <stdlib.h>

/// Shorthand allocation.
#ifndef ALLOC
#define ALLOC(mp_ptr, mp_size) \
    realloc((mp_ptr), (mp_size))
#endif // ALLOC

/// Shorthand typed allocation.
#define ALLOC_TYPED(id_type) \
    (id_type *)ALLOC(NULL, sizeof(id_type))

/// Shorthand free.
#ifndef FREE
#define FREE(mp_allocated) \
    free((void *)mp_allocated)
#endif // FREE

#endif // CONTAINER_ALLOCA_H