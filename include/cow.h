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

/// Set `mp_status` to `mp_message` (a c-string error message) and go to `id_label`,
/// when `mp_condition` evaluate to true.
/// It is used to raise error.
#define RAISE(mp_status, id_label, mp_condition, mp_message)                                                                     \
    if (mp_condition)                                                                                                            \
    {                                                                                                                            \
        (mp_status) = COW(false, "[Error in " __FILE__ ", line " MACRO_STRINGIFY(__LINE__) "]: " mp_message "\n", sizeof(char)); \
        goto id_label;                                                                                                           \
    }

/// Print formatted `mp_message` and `mp_args` to `stderr`,
/// set `mp_status.data.ptr` to `1` (panic pointer),
/// when `mp_condition` evaluate to true.
/// It is used only when unrecoverable error.
/// It prints out the error immediately instead of saving it into `cow_t`.
#define PANIC(mp_status, id_label, mp_condition, mp_message...)                                      \
    if (mp_condition)                                                                                \
    {                                                                                                \
        (mp_status) = COW(false, (void *)1, 0);                                                      \
        fprintf(stderr, "[Panic in " __FILE__ ", line " MACRO_STRINGIFY(__LINE__) "]: " mp_message); \
        fputc('\n', stderr);                                                                         \
        goto id_label;                                                                               \
    }

/// Set result of `mp_expression` to `mp_status`.
/// Go to `id_label` if `mp_status` having valid data.
/// It is used to catch and/or rethrow error.
/// It immediately print out traceback during panic.
#define TRY(mp_status, id_label, mp_expression)                                                   \
    if (((mp_status) = (mp_expression)).data.ptr != NULL)                                         \
    {                                                                                             \
        if ((mp_status).data.ptr == (void *)1)                                                    \
        { /* Panic. */                                                                            \
            fputs("\t- Called from " __FILE__ ", line " MACRO_STRINGIFY(__LINE__) ".\n", stderr); \
        }                                                                                         \
        goto id_label;                                                                            \
    }

#endif // CONTAINER_COW_H