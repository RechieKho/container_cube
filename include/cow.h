#ifndef CONTAINER_COW_H
#define CONTAINER_COW_H

#include <stdbool.h>
#include <stdio.h>
#include <data.h>
#include <macros.h>

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

#endif // CONTAINER_COW_H