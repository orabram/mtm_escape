#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "escaperoom.h"

#define ASSERT_TEST(b) do { \
        if (!(b)) { \
                printf("\nAssertion failed at %s:%d %s\n",__FILE__,__LINE__,#b); \
                return false; \
        } \
} while (0)


static SetElement copy_room(SetElement room)
{
    EscapeRoom copy = create_escape_room();
    if (copy == NULL) {
        return NULL;
    }
    if (escape_room_copy(copy, room) != MTM_SUCCESS) {
        escape_room_destroy(copy);
        return NULL;
    }
    return copy;
}

static void destroy_room(SetElement room)
{
    escape_room_destroy(room);
}

static int compare_room(SetElement room1, SetElement room2)
{
    EscapeRoom r1 = (EscapeRoom)room1, r2 = (EscapeRoom)room2;
    return escape_room_get_id(r1) - escape_room_get_id(r2);
}


static





int main()
{
    ASSERT_TEST(true);
}
