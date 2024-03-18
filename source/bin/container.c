#include <stdlib.h>
#include <list.h>
#include <error.h>

int main()
{
    ERROR_START(status);

    list_t message = LIST(sizeof(char));
    TRY(status, end, list_string_from(&message, "Hello world, %d", 135));
    TRY(status, end, list_push(&message, DATA("a", sizeof(char))));
    TRY(status, end, list_push(&message, DATA("b", sizeof(char))));
    TRY(status, end, list_push(&message, DATA("\n", sizeof(char))));
    TRY(status, end, slice_print(&message.slice, stdout));

end:
    if (status.slice.data.ptr != NULL && status.slice.data.ptr != (void *)1)
        slice_print(&status.slice, stderr);

    ERROR_END(status);

    return 0;
}