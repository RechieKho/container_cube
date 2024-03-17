#include <slice.h>

#include <stdint.h>

cow_t slice_slice(const slice_t *p_slice, size_t p_begin, size_t p_end, slice_t *r_slice)
{
    // Bad arguments.
    if (p_slice == NULL)
        return COW(true, "Input slice (`p_slice`) is `NULL`.", sizeof(char));

    if (p_begin > p_slice->length)
        return COW(true, "Begining index (`p_begin`) is out of bound.", sizeof(char));

    if (p_end > p_slice->length)
        return COW(true, "Ending index (`p_end`) is out of bound.", sizeof(char));

    if (p_begin > p_end)
        return COW(true, "Begining index (`p_begin`) is greater than ending index (`p_end`).", sizeof(char));

    if (r_slice == NULL)
        return COW(true, "Returning slice (`r_slice`) is `NULL`.", sizeof(char));

    if (p_slice->data.ptr == NULL)
        return COW(true, "Input slice's data (`p_slice->data.ptr`) is `NULL`.", sizeof(char));

    // Initialize return value.
    *r_slice = (slice_t){0};

    // Calculate
    uint8_t *const casted_ptr = (uint8_t *)p_slice->data.ptr;
    uint8_t *const begin_ptr = casted_ptr + (p_slice->data.size * p_begin);
    size_t length = p_begin - p_end;

    // Return value.
    r_slice->data.ptr = (void *)begin_ptr;
    r_slice->data.size = p_slice->data.size;
    r_slice->length = length;

    return (cow_t){0};
}

cow_t slice_borrow(const slice_t *p_slice, size_t p_index, data_t *r_data)
{
    // Bad arguments.
    if (p_slice == NULL)
        return COW(true, "Input slice (`p_slice`) is `NULL`.", sizeof(char));

    if (p_index > p_slice->length)
        return COW(true, "Index (`p_index`) is out of bound.", sizeof(char));

    if (r_data == NULL)
        return COW(true, "Returning data (`r_data`) is `NULL`.", sizeof(char));

    if (p_slice->data.ptr == NULL)
        return COW(true, "Input slice's data (`p_slice->data.ptr`) is `NULL`.", sizeof(char));

    // Initialize return value.
    *r_data = (data_t){0};

    // Calculate.
    uint8_t *const casted_ptr = (uint8_t *)p_slice->data.ptr;
    uint8_t *const new_ptr = casted_ptr + (p_slice->data.size * p_index);

    // Return value.
    r_data->ptr = (void *)new_ptr;
    r_data->size = p_slice->data.size;

    return (cow_t){0};
}

cow_t slice_print(const slice_t *p_slice, FILE *m_file)
{
    // Bad arguments.
    if (p_slice == NULL)
        return COW(true, "Input slice (`p_slice`) is `NULL`.", sizeof(char));

    if (m_file == NULL)
        return COW(true, "Target file (`m_file`) is `NULL`.", sizeof(char));

    DATA_CAST(char, slice_char_ptr, p_slice->data);
    if (slice_char_ptr == NULL)
        return COW(true, "Unable to cast data of slice (`p_slice->data.ptr`) to `char` due to incompatible size.", sizeof(char));

    fprintf(m_file, "%.*s", (int)p_slice->length, *slice_char_ptr);

    return (cow_t){0};
}