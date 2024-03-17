#include <list.h>

#include <string.h>
#include <alloc.h>
#include <stdlib.h>
#include <stdarg.h>

#define INITIAL_CAPACITY 10

cow_t list_reserve(list_t *m_list, size_t p_min_capacity)
{
    // Bad arguments.
    if (m_list == NULL)
        return COW(true, "Target list (`m_list`) is `NULL`.", sizeof(char));

    // Calculate new capacity.
    size_t new_capacity = m_list->capacity == 0 ? INITIAL_CAPACITY : m_list->capacity;
    while (new_capacity < p_min_capacity)
        new_capacity *= 2;

    // Reallocate new memory.
    if ((m_list->slice.data.ptr = ALLOC(m_list->slice.data.ptr, new_capacity * m_list->slice.data.size)) == NULL)
        return COW(true, "Fail to allocate memory.", sizeof(char));

    m_list->capacity = new_capacity;

    return (cow_t){0};
}

cow_t list_clean(list_t *m_list)
{
    // Bad arguments.
    if (m_list == NULL)
        return COW(true, "Target list (`m_list`) is `NULL`.", sizeof(char));

    if (m_list->slice.data.ptr != NULL)
        free(m_list->slice.data.ptr);
    m_list->capacity = 0;
    m_list->slice.length = 0;
    m_list->slice.data.ptr = NULL;

    return (cow_t){0};
}

cow_t list_push(list_t *m_list, data_t p_data)
{
    // Bad arguments.
    if (m_list == NULL)
        return COW(true, "Target list (`m_list`) is `NULL`.", sizeof(char));

    if (p_data.ptr == NULL)
        return COW(true, "Input data's pointer (`p_data.ptr`) is `NULL`.", sizeof(char));

    if (p_data.size != m_list->slice.data.size)
        return COW(true, "The size of input data (`p_data`) and target list (`m_list`) is incompatible.", sizeof(char));

    // Initialize status.
    cow_t status = (cow_t){0};

    // Allocate memory.
    if ((status = list_reserve(m_list, m_list->slice.length + 1)).data.ptr != NULL)
        return status;

    uint8_t *const casted_ptr = (uint8_t *)m_list->slice.data.ptr;
    uint8_t *const indexed_ptr = casted_ptr + (m_list->slice.length * m_list->slice.data.size);
    memcpy((void *)indexed_ptr, p_data.ptr, p_data.size);
    m_list->slice.length += 1;

    return status;
}

cow_t list_string_from(list_t *r_string, const char *p_message, ...)
{
    // Bad arguments.
    if (r_string == NULL)
        return COW(true, "Returning list (`r_string`) is `NULL`.", sizeof(char));

    // Initialize status.
    cow_t status = (cow_t){0};

    // Initialize returning values.
    *r_string = (list_t){0};

    // Initialize variadic arguments.
    va_list args;
    va_start(args, p_message);

    // Calculate the length.
    size_t length = vsnprintf(NULL, 0, p_message, args) + 1; // Include null-terminator.

    // Initialize list.
    list_t list = LIST(sizeof(char));
    if ((status = list_reserve(&list, length)).data.ptr != NULL)
        goto cleanup_va;
    DATA_CAST(char, list_char_ptr, list.slice.data);
    if (list_char_ptr == NULL)
    {
        status = COW(true, "Unable to cast list with size of `char` to `char`. [Internal error]", sizeof(char));
        goto cleanup_va;
    }

    // Write to list.
    vsnprintf(*list_char_ptr, length, p_message, args); // `vsnprintf` will always write null-terminator.
    list.slice.length = length - 1;                     // Exclude null-terminator.

    // Return values.
    *r_string = list;

    // Clean up.
cleanup_va:
    va_end(args);

    if (!status.borrowed && status.data.ptr != NULL)
        FREE(status.data.ptr);

    return status;
}