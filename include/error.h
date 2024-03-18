#ifndef CONTAINER_ERROR_H
#define CONTAINER_ERROR_H

#include <list.h>
#include <macros.h>

/// Error is as simple as a `list_t` string that contains an error message.
/// If `list_t.slice.data.ptr` is `NULL`, then it is a success,
/// else, it failed.

/// Create a variable with identifier `id_error` that stores error.
#define ERROR_START(id_error) \
    list_t id_error = LIST(sizeof(char))

/// Clean up the error variable.
#define ERROR_END(mp_error) \
    list_clean(&(mp_error))

/// Print formatted `mp_message` and `mp_args` to `stderr`,
/// set `mp_error.slice.data.ptr` to `1` (panic pointer),
/// when `mp_condition` evaluate to true.
/// It is used only when unrecoverable error.
/// It prints out the error immediately instead of saving it into `list_t`.
#define PANIC(mp_error, id_label, mp_condition, mp_message...)                                       \
    if (mp_condition)                                                                                \
    {                                                                                                \
        (mp_error).slice.data.ptr = (void *)1;                                                       \
        fprintf(stderr, "[Panic in " __FILE__ ", line " MACRO_STRINGIFY(__LINE__) "]: " mp_message); \
        fputc('\n', stderr);                                                                         \
        goto id_label;                                                                               \
    }

/// Set `mp_error` to `mp_message` (a c-string error message) and go to `id_label`,
/// when `mp_condition` evaluate to true.
/// It is used to raise error.
#define RAISE(mp_error, id_label, mp_condition, mp_message...)                                                                              \
    if (mp_condition)                                                                                                                       \
    {                                                                                                                                       \
        TRY(mp_error, id_label, list_string_from(&(mp_error), "[Error in " __FILE__ ", line " MACRO_STRINGIFY(__LINE__) "]: " mp_message)); \
        TRY(mp_error, id_label, list_push(&(mp_error), DATA("\n", sizeof(char))));                                                          \
        goto id_label;                                                                                                                      \
    }

/// Set result of `mp_expression` to `mp_error`.
/// Go to `id_label` if `mp_error` having valid data.
/// It is used to catch and/or rethrow error.
/// It immediately print out traceback during panic.
#define TRY(mp_error, id_label, mp_expression)                                                    \
    if (((mp_error) = (mp_expression)).slice.data.ptr != NULL)                                    \
    {                                                                                             \
        if ((mp_error).slice.data.ptr == (void *)1)                                               \
        { /* Panic. */                                                                            \
            fputs("\t- Called from " __FILE__ ", line " MACRO_STRINGIFY(__LINE__) ".\n", stderr); \
        }                                                                                         \
        goto id_label;                                                                            \
    }

#endif // CONTAINER_ERROR_H