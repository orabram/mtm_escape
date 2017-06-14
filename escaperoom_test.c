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
    ASSERT_TEST(initialize_escape_room(room, "helloworld.com", 1, 72, 5,
                                       "10-22", 3) == MTM_INVALID_PARAMETER);
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
    char* working_hrs = get_room_working_hrs(room);
    ASSERT_TEST(strcmp(working_hrs, "10-22") == 0);
    ASSERT_TEST(escape_room_get_difficulty(room) == 3);
    free(working_hrs);
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

static bool test_find_closest_time()
{
    EscapeRoom room = create_escape_room();
    ASSERT_TEST(initialize_escape_room(room, "hello@world.com", 1, 72, 5,
                                       "10-22", 3) == MTM_SUCCESS);
    int day, hour;
    ASSERT_TEST(escape_room_find_closest_time(room, &day, &hour)==MTM_SUCCESS);
    ASSERT_TEST(day == 0 && hour == 10);
    Order order = create_order();
    initialize_order(order, "client@mtm.com", ELECTRICAL_ENGINEERING, 57,
                     "0-10", 3);
    ASSERT_TEST(escape_room_add_order(room, order) == MTM_SUCCESS);
    ASSERT_TEST(escape_room_find_closest_time(room, &day, &hour)==MTM_SUCCESS);
    ASSERT_TEST(day == 0 && hour == 11);
    return true;
}

static bool test_add_order()
{
    EscapeRoom room = create_escape_room();
    ASSERT_TEST(initialize_escape_room(room, "hello@world.com", 1, 72, 5,
                                       "10-22", 3) == MTM_SUCCESS);
    ASSERT_TEST(escape_room_add_order(room, NULL) == MTM_NULL_PARAMETER);
    Order order1 = create_order();
    initialize_order(order1, "client1@mtm.com", ELECTRICAL_ENGINEERING, 57,
                     "2-15", 3);
    ASSERT_TEST(escape_room_add_order(room, order1) == MTM_SUCCESS);
    order_remove(order1);
    Order order2 = create_order();
    initialize_order(order2, "client2@mtm.com", CIVIL_ENGINEERING, 45, "1-9",4);
    //Order 2 is trying to reserve at illegal hour
    ASSERT_TEST(escape_room_add_order(room, order2) == MTM_ROOM_NOT_AVAILABLE);
    order_remove(order2);
    Order order3 = create_order();
    initialize_order(order3, "client3@mtm.com", AEROSPACE_ENGINEERING, 23,
                     "2-15", 8);
    //Order 3 is trying to reserve at the same hours as Order 1
    ASSERT_TEST(escape_room_add_order(room, order3) == MTM_ROOM_NOT_AVAILABLE);
    order_remove(order3);
    escape_room_destroy(room);
    return true;
}

static bool test_remove_order()
{
    EscapeRoom room = create_escape_room();
    ASSERT_TEST(escape_room_remove_order(room, NULL) == MTM_NULL_PARAMETER);
    ASSERT_TEST(initialize_escape_room(room, "hello@world.com", 1, 72, 5,
                                       "10-22", 3) == MTM_SUCCESS);
    Order order = create_order();
    initialize_order(order, "client1@mtm.com", ELECTRICAL_ENGINEERING, 57,
                     "2-15", 3);
    ASSERT_TEST(escape_room_remove_order(NULL, order) == MTM_NULL_PARAMETER);
    ASSERT_TEST(escape_room_remove_order(room, order) ==
                MTM_INVALID_PARAMETER);
    ASSERT_TEST(escape_room_add_order(room, order) == MTM_SUCCESS);
    ASSERT_TEST(escape_room_remove_order(room, order) == MTM_SUCCESS);
    escape_room_destroy(room);
    order_remove(order);
    return true;
}

static bool test_order_exists()
{
    ASSERT_TEST(!escape_room_order_exists(NULL));
    EscapeRoom room = create_escape_room();
    ASSERT_TEST(initialize_escape_room(room, "hello@world.com", 1, 72, 5,
                                       "10-22", 3) == MTM_SUCCESS);
    ASSERT_TEST(!escape_room_order_exists(room));
    Order order1 = create_order();
    initialize_order(order1, "client1@mtm.com", ELECTRICAL_ENGINEERING, 57,
                     "2-15", 3);
    ASSERT_TEST(escape_room_add_order(room, order1) == MTM_SUCCESS);
    ASSERT_TEST(escape_room_order_exists(room));
    ASSERT_TEST(escape_room_remove_order(room, order1) == MTM_SUCCESS);
    order_remove(order1);
    Order order2 = create_order();
    initialize_order(order2, "client2@mtm.com", CIVIL_ENGINEERING, 45, "1-9",4);
    //Order 2 is trying to reserve at illegal hour
    ASSERT_TEST(escape_room_add_order(room, order2) == MTM_ROOM_NOT_AVAILABLE);
    ASSERT_TEST(!escape_room_order_exists(room));
    order_remove(order2);
    escape_room_destroy(room);
    return true;
}

int main()
{
    RUN_TEST(test_create_escape_room);
    RUN_TEST(test_initialize_and_copy);
    RUN_TEST(test_get_functions);
    RUN_TEST(test_recommended_value);
    RUN_TEST(test_find_closest_time);
    RUN_TEST(test_add_order);
    RUN_TEST(test_remove_order);
    RUN_TEST(test_order_exists);
    printf("\nAll tests passed!\n");
    return 0;
}
