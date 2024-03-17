#include <list.h>

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
    size_t length = vsnprintf(NULL, 0, p_message, args) + 1;

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
    vsnprintf(*list_char_ptr, length, p_message, args);
    list.slice.length = length;

    // Return values.
    *r_string = list;

    // Clean up.
cleanup_va:
    va_end(args);

    if (!status.borrowed && status.data.ptr != NULL)
        FREE(status.data.ptr);

    return status;
}