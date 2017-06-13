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

#define RUN_TEST(name)  printf("Running "); \
						printf(#name);		\
						printf("... ");		\
						if (!name()) { \
							printf("[FAILED]\n");		\
							return false; \
						}								\
						printf("[SUCCESS]\n");


/*static SetElement copy_room(SetElement room)
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
}*/


static bool test_create_escape_room()
{
    EscapeRoom room = create_escape_room();
    ASSERT_TEST(room != NULL);
    escape_room_destroy(room);
    return true;
}

static bool test_initialize_and_copy()
{
    EscapeRoom room = create_escape_room();
    ASSERT_TEST(initialize_escape_room(NULL, "hello@world.com", 1, 72, 5,
                                       "10-22", 3) == MTM_NULL_PARAMETER);
    ASSERT_TEST(initialize_escape_room(room, "hello@world.com", 1, 72, 5,
                                       "10-22", 3) == MTM_SUCCESS);
    EscapeRoom copy = create_escape_room();
    ASSERT_TEST(escape_room_copy(copy, room) == MTM_SUCCESS);
    escape_room_destroy(room);
    escape_room_destroy(copy);
    return true;
}

static bool test_get_functions()
{
    EscapeRoom room = create_escape_room();
    ASSERT_TEST(initialize_escape_room(room, "hello@world.com", 1, 72, 5,
                                       "10-27", 3) == MTM_INVALID_PARAMETER);
    ASSERT_TEST(initialize_escape_room(room, "hello@world.com", 1, 72, 5,
                                       "23-22", 3) == MTM_INVALID_PARAMETER);
    ASSERT_TEST(initialize_escape_room(room, "hello@world.com", 1, 72, 5,
                                       "10-22", 3) == MTM_SUCCESS);
    char* email = escape_room_get_email(room);
    ASSERT_TEST(email != NULL);
    ASSERT_TEST(strcmp(email, "hello@world.com") == 0);
    ASSERT_TEST(escape_room_get_id(room) == 1);
    ASSERT_TEST(escape_room_get_price(room) == 72);
    ASSERT_TEST(get_room_working_hrs(room) == "10-22");
    escape_room_destroy(room);
    return true;
}

static bool test_recommended_value()
{
    EscapeRoom room = create_escape_room();
    ASSERT_TEST(initialize_escape_room(room, "hello@world.com", 1, 72, 5,
                                       "10-22", 3) == MTM_SUCCESS);
    /**
     * The expected result is determined by the formula:
     * (P_r - P_e)^2 + (difficulty - skill level)^2
     * Substituting the parameters with our values, we get the result:
     * (5 - 8)^2 + (3 - 2)^2 = 10
     */
    int calc_expected = 10;

    ASSERT_TEST(escape_room_calculate_recommended_value(room, 2, 8) ==
                        calc_expected);
    escape_room_destroy(room);
    return true;
}

static bool test_add_order()
{
    EscapeRoom room = create_escape_room();
    ASSERT_TEST(escape_room_add_order(room, NULL) == MTM_NULL_PARAMETER);
    Order order = create_order();
    initialize_order(order, "order@mtm.com", ELECTRICAL_ENGINEERING, 57,
                     "2-15", 3);
    ASSERT_TEST(escape_room_add_order(room, order) == MTM_SUCCESS);

}





int main()
{
    RUN_TEST(test_create_escape_room);
    RUN_TEST(test_initialize_and_copy);
    RUN_TEST(test_get_functions);
    RUN_TEST(test_recommended_value);
    return 0;
}
