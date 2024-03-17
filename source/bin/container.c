#include <stdlib.h>
#include <list.h>

#define CATCH(mp_expression)                             \
    {                                                    \
        cow_t status = (cow_t){0};                       \
        if ((status = (mp_expression)).data.ptr != NULL) \
        {                                                \
            printf("%s", (char *)status.data.ptr);       \
            exit(1);                                     \
        }                                                \
    }

int main()
{
    list_t message = LIST(sizeof(char));
    CATCH(list_string_from(&message, "Hello world, %d", 135));
    CATCH(list_push(&message, DATA("a", sizeof(char))));
    CATCH(list_push(&message, DATA("\n", sizeof(char))));
    CATCH(slice_print(&message.slice, stdout));
    return 0;
}