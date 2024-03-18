#include <list.h>

#include <string.h>
#include <alloc.h>
#include <stdlib.h>
#include <stdarg.h>
#include <error.h>

#define INITIAL_CAPACITY 16

error_t list_reserve(list_t *m_list, size_t p_min_capacity)
{
    // Initialize error.
    ERROR_START(error);

    // Bad arguments.
    PANIC(error, end, m_list == NULL, "Target list (`m_list`) is `NULL`.");

    // Calculate new capacity.
    size_t new_capacity = m_list->capacity == 0 ? INITIAL_CAPACITY : m_list->capacity;
    while (new_capacity < p_min_capacity)
        new_capacity *= 2;

    // Reallocate new memory.
    m_list->slice.data.ptr = ALLOC(m_list->slice.data.ptr, new_capacity * m_list->slice.data.size);
    PANIC(error, end, m_list->slice.data.ptr == NULL, "Fail to allocate `%zu` bytes memory.", new_capacity * m_list->slice.data.size);

    m_list->capacity = new_capacity;

end:
    return error;
}

error_t list_clean(list_t *m_list)
{
    // Initialize error.
    ERROR_START(error);

    // Bad arguments.
    PANIC(error, end, m_list == NULL, "Target list (`m_list`) is `NULL`.");

    if (m_list->slice.data.ptr != NULL)
        free(m_list->slice.data.ptr);
    m_list->capacity = 0;
    m_list->slice.length = 0;
    m_list->slice.data.ptr = NULL;

end:
    return error;
}

error_t list_extend(list_t *m_list, size_t p_new_length)
{
    // Initialize error.
    ERROR_START(error);

    // Bad arguments.
    PANIC(error, end, m_list == NULL, "Target list (`m_list`) is `NULL`.");

    // Early return.
    if (m_list->slice.length >= p_new_length)
        return error;

    // Allocate memory.
    TRY(error, end, list_reserve(m_list, p_new_length));

    // Extends.
    m_list->slice.length = p_new_length;

end:
    return error;
}

error_t list_push(list_t *m_list, data_t p_data)
{
    // Initialize error.
    ERROR_START(error);

    // Bad arguments.
    PANIC(error, end, m_list == NULL, "Target list (`m_list`) is `NULL`.");
    PANIC(error, end, p_data.ptr == NULL, "Input data's pointer (`p_data.ptr`) is `NULL`.");
    PANIC(error, end, p_data.size != m_list->slice.data.size, "The size of input data (`p_data`) and target list (`m_list`) is incompatible.");

    // Extends
    TRY(error, end, list_extend(m_list, m_list->slice.length + 1));
    TRY(error, end, slice_set(&m_list->slice, m_list->slice.length - 1, p_data));

end:
    return error;
}

error_t list_string_append(list_t *m_string, const char *p_message, ...)
{
    // Initialize error.
    ERROR_START(error);

    // Bad arguments.
    PANIC(error, end, m_string == NULL, "Target list (`m_string`) is `NULL`.");
    PANIC(error, end, m_string->slice.data.size != sizeof(char), "The size of target list (`m_string`) and the size of character is incompatible");

    // Initialize variadic arguments.
    va_list args;
    va_start(args, p_message);

    // Calculate the length.
    size_t length = vsnprintf(NULL, 0, p_message, args) + 1; // Include null-terminator.

    // Initialize list.
    TRY(error, cleanup_va, list_reserve(m_string, length));
    DATA_CAST(char, string_char_ptr, m_string->slice.data);
    RAISE(error, cleanup_va, string_char_ptr == NULL, "Unable to cast list with size of `char` to `char`. [Internal error]");

    // Write to list.
    vsnprintf(&(*string_char_ptr)[m_string->slice.length], length, p_message, args); // `vsnprintf` will always write null-terminator.
    m_string->slice.length += length - 1;                                            // Exclude null-terminator.

    // Clean up.
cleanup_va:
    va_end(args);

end:
    return error;
}