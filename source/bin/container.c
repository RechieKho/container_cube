#include <stdlib.h>
#include <list.h>

int main()
{
    cow_t status = (cow_t){0};

    list_t message = LIST(sizeof(char));
    TRY(status, end, list_string_from(&message, "Hello world, %d", 135));
    TRY(status, end, list_push(&message, DATA("a", sizeof(char))));
    TRY(status, end, list_push(&message, DATA("\n", sizeof(char))));
    TRY(status, end, slice_print(&message.slice, stdout));

end:
    if (status.data.ptr != NULL)
        printf("%s\n", (char *)status.data.ptr);

    return 0;
}