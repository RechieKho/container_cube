#ifndef CONTAINER_SLICE_H
#define CONTAINER_SLICE_H

#include <stdio.h>
#include <stdbool.h>
#include <data.h>
#include <cow.h>

/// A slice of data.
typedef struct slice_t
{
    data_t data;
    size_t length;
} slice_t;

#define SLICE(mp_ptr, mp_size, mp_length) \
    (slice_t)                             \
    {                                     \
        .data = (data_t){                 \
            .ptr = (mp_ptr),              \
            .size = (mp_size)},           \
        .length = (mp_length)             \
    }

/// Slice a `slice_t`.
/// Return a message (`const char*`) on error, else NULL.
cow_t slice_slice(const slice_t *p_slice, size_t p_begin, size_t p_end, slice_t *r_slice);

/// Index a `slice_t`.
/// Return a message (`const char*`) on error, else NULL.
cow_t slice_borrow(const slice_t *p_slice, size_t p_index, data_t *r_data);

/// Print out the slice of characters to file. The slice must be having data's size same as character's size.
/// Return a message (`const char*`) on error, else NULL.
cow_t slice_print(const slice_t *p_slice, FILE *m_file);

#endif // CONTAINER_SLICE_H