#ifndef CONTAINER_ALLOC_H
#define CONTAINER_ALLOC_H

#include <stdlib.h>

#ifndef MALLOC 
    #define MALLOC malloc
#endif

#ifndef CALLOC 
    #define CALLOC calloc
#endif

#ifndef REALLOC 
    #define REALLOC realloc
#endif

#ifndef FREE
    #define FREE_CUBE free
#endif

/// Shorthand allocation.
#define ALLOC(mp_ptr, mp_size) \
    REALLOC((mp_ptr), (mp_size))

/// Shorthand typed allocation.
#define ALLOC_TYPED(id_type) \
    (id_type *)CALLOC(1, sizeof(id_type))

/// Shorthand free.
#ifndef FREE_CUBE
#define FREE(mp_allocated) \
    FREE_CUBE((void *)mp_allocated)

#endif

#endif // CONTAINER_ALLOCA_H