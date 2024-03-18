#include <slice.h>

#include <stdint.h>
#include <error.h>

list_t slice_slice(const slice_t *p_slice, size_t p_begin, size_t p_end, slice_t *r_slice)
{
    // Initialize error.
    ERROR_START(error);

    // Bad arguments.
    PANIC(error, end, p_slice == NULL, "Input slice (`p_slice`) is `NULL`.");
    PANIC(error, end, p_begin > p_slice->length, "Begining index (`p_begin`) is out of bound.");
    PANIC(error, end, p_end > p_slice->length, "Ending index (`p_end`) is out of bound.");
    PANIC(error, end, p_begin > p_end, "Begining index (`p_begin`) is greater than ending index (`p_end`).");
    PANIC(error, end, r_slice == NULL, "Returning slice (`r_slice`) is `NULL`.");
    PANIC(error, end, p_slice->data.ptr == NULL, "Input slice's data (`p_slice->data.ptr`) is `NULL`.");

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

end:
    return error;
}

list_t slice_borrow(const slice_t *p_slice, size_t p_index, data_t *r_data)
{
    // Initialize error.
    ERROR_START(error);

    // Bad arguments.
    PANIC(error, end, p_slice == NULL, "Input slice (`p_slice`) is `NULL`.");
    PANIC(error, end, p_index > p_slice->length, "Index (`p_index`) is out of bound.");
    PANIC(error, end, r_data == NULL, "Returning data (`r_data`) is `NULL`.");
    PANIC(error, end, p_slice->data.ptr == NULL, "Input slice's data (`p_slice->data.ptr`) is `NULL`.");

    // Initialize return value.
    *r_data = (data_t){0};

    // Calculate.
    uint8_t *const casted_ptr = (uint8_t *)p_slice->data.ptr;
    uint8_t *const new_ptr = casted_ptr + (p_slice->data.size * p_index);

    // Return value.
    r_data->ptr = (void *)new_ptr;
    r_data->size = p_slice->data.size;

end:
    return error;
}

list_t slice_print(const slice_t *p_slice, FILE *m_file)
{
    // Initialize error.
    ERROR_START(error);

    // Bad arguments.
    PANIC(error, end, p_slice == NULL, "Input slice (`p_slice`) is `NULL`.");
    PANIC(error, end, m_file == NULL, "Target file (`m_file`) is `NULL`.");

    DATA_CAST(char, slice_char_ptr, p_slice->data);
    RAISE(error, end, slice_char_ptr == NULL, "Unable to cast data of slice (`p_slice->data.ptr`) to `char` due to incompatible size.");

    fprintf(m_file, "%.*s", (int)p_slice->length, *slice_char_ptr);

end:
    return error;
}