#include <stdlib.h>
#include <list.h>
#include <error.h>

int main()
{
    ERROR_START(error);

    list_t message = LIST(sizeof(char));
    TRY(error, end, list_string_append(&message, "Hello world, %d", 135));
    TRY(error, end, list_push(&message, DATA("b", sizeof(char))));
    TRY(error, end, list_push(&message, DATA("\n", sizeof(char))));
    TRY(error, end, slice_print(&message.slice, stdout));

end:
    ERROR_PRINT(error)
    ERROR_END(error);

    return 0;
}