#ifndef CONTAINER_ERROR_H
#define CONTAINER_ERROR_H

#include <list.h>
#include <macros.h>

/// Error is as simple as a `list_t` string that contains an error message.
/// If `list_t.slice.data.ptr` is `NULL`, then it is a success,else it failed.
/// If it is an error, it will save the error. The programmer have liberty to handle it using their own way.
/// If it is a panic, the message will immediately log to `stderr` and `list_t.slice.data.ptr` sets to `1`.
/// But since error is saved, the action of saving itself might cause error as well.
/// Thus, use panic unless you want the caller have an option to handle the error.

/// Create a variable with identifier `id_error` that stores error.
#define ERROR_START(id_error) \
    list_t id_error = LIST(sizeof(char));

/// Print out the error if available.
#define ERROR_PRINT(mp_error)                                                        \
    if ((mp_error).slice.data.ptr != NULL && (mp_error).slice.data.ptr != (void *)1) \
        slice_print(&(mp_error).slice, stderr);

/// Clean up the error variable if available.
#define ERROR_END(mp_error)                                          \
    if ((mp_error).slice.data.ptr != (void *)1) /* Exclude panic. */ \
        list_clean(&(mp_error));

/// Print formatted `mp_message` and `mp_args` to `stderr`,
/// set `mp_error.slice.data.ptr` to `1` (panic pointer),
/// when `mp_condition` evaluate to true.
/// It prints out the error immediately instead of saving it into `list_t`.
#define PANIC(mp_error, id_label, mp_condition, mp_message...)                                       \
    if (mp_condition)                                                                                \
    {                                                                                                \
        (mp_error).slice.data.ptr = (void *)1;                                                       \
        fprintf(stderr, "[Panic in " __FILE__ ", line " MACRO_STRINGIFY(__LINE__) "]: " mp_message); \
        fputc('\n', stderr);                                                                         \
        goto id_label;                                                                               \
    }

/// Set result of `mp_expression` to `mp_error`.
/// Go to `id_label` if `mp_error` having valid data.
/// It is used to catch and/or rethrow error.
/// It immediately print out traceback during panic,
/// else it saves the traceback to the to `mp_error`.
#define TRY(mp_error, id_label, mp_expression)                                                                                        \
    if (((mp_error) = (mp_expression)).slice.data.ptr != NULL)                                                                        \
    {                                                                                                                                 \
        if ((mp_error).slice.data.ptr == (void *)1)                                                                                   \
        {                                                                                                                             \
            /* Panic. */                                                                                                              \
            fputs("\t- Called from " __FILE__ ", line " MACRO_STRINGIFY(__LINE__) ".\n", stderr);                                     \
            goto id_label;                                                                                                            \
        }                                                                                                                             \
        else                                                                                                                          \
        {                                                                                                                             \
            list_t m_result = list_string_append(&(mp_error), "\t- Called from " __FILE__ ", line " MACRO_STRINGIFY(__LINE__) ".\n"); \
            PANIC(mp_error, id_label, m_result.slice.data.ptr != NULL, "Fail to save backtrace.");                                    \
            goto id_label;                                                                                                            \
        }                                                                                                                             \
    }

/// Set `mp_error` to `mp_message` (a c-string error message) and go to `id_label`,
/// when `mp_condition` evaluate to true.
/// It is used to raise error.
/// It saves the error to `mp_error`.
/// It will save its own error instead if itself fails.
#define RAISE(mp_error, id_label, mp_condition, mp_message...)                                                                            \
    if (mp_condition)                                                                                                                     \
    {                                                                                                                                     \
        list_t m_error = LIST(sizeof(char));                                                                                              \
        TRY(m_error, m_end, list_clean(&(mp_error)));                                                                                     \
        TRY(m_error, m_end, list_string_append(&(mp_error), "[Error in " __FILE__ ", line " MACRO_STRINGIFY(__LINE__) "]: " mp_message)); \
        TRY(m_error, m_end, list_push(&(mp_error), DATA("\n", sizeof(char))));                                                            \
    m_end:                                                                                                                                \
        if (m_error.slice.data.ptr != NULL)                                                                                               \
            (mp_error) = m_error;                                                                                                         \
        goto id_label;                                                                                                                    \
    }

#endif // CONTAINER_ERROR_H