#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "company.h"

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

static bool test_create_company()
{
    Company company = create_company();
    ASSERT_TEST(company != NULL);
    company_destroy(company);
    return true;
}

static bool test_initialize_and_copy()
{
    Company company = create_company();
    ASSERT_TEST(initialize_company(company, "companymtm.com",
                                   BIOMEDICAL_ENGINEERING) ==
                MTM_INVALID_PARAMETER);
    ASSERT_TEST(initialize_company(company, "company@mtm.com",
                                   BIOMEDICAL_ENGINEERING) == MTM_SUCCESS);
    Company copy = company_copy(company);
    ASSERT_TEST(copy != NULL);
    company_destroy(company);
    company_destroy(copy);
    return true;
}

static bool test_add_room()
{
    Company company = create_company();
    ASSERT_TEST(initialize_company(company, "company@mtm.com",
                                   BIOMEDICAL_ENGINEERING) == MTM_SUCCESS);
    ASSERT_TEST(company_add_room(company, NULL) == MTM_NULL_PARAMETER);
    EscapeRoom room1 = create_escape_room();
    ASSERT_TEST(initialize_escape_room(room1, "company@mtm.com", 32, 80, 4,
                                       "12-21", 5) == MTM_SUCCESS);
    ASSERT_TEST(company_add_room(NULL, room1) == MTM_NULL_PARAMETER);
    ASSERT_TEST(company_add_room(company, room1) == MTM_SUCCESS);
    escape_room_destroy(room1);
    EscapeRoom room2 = create_escape_room();
    ASSERT_TEST(initialize_escape_room(room2, "company2@mtm.com", 33, 112, 6,
                                       "09-19", 2) == MTM_SUCCESS);
    ASSERT_TEST(company_add_room(company, room2) == MTM_INVALID_PARAMETER);
    escape_room_destroy(room2);
    EscapeRoom room3 =  create_escape_room();
    ASSERT_TEST(initialize_escape_room(room3, "company@mtm.com", 32, 112, 6,
                                       "09-19", 2) == MTM_SUCCESS);
    ASSERT_TEST(company_add_room(company, room3) == MTM_ID_ALREADY_EXIST);
    escape_room_destroy(room3);
    company_destroy(company);
    return true;
}

static bool test_remove_room()
{
    Company company = create_company();
    ASSERT_TEST(initialize_company(company, "company@mtm.com",
                                   BIOMEDICAL_ENGINEERING) == MTM_SUCCESS);
    ASSERT_TEST(company_remove_room(NULL, 32) == MTM_NULL_PARAMETER);
    EscapeRoom room = create_escape_room();
    ASSERT_TEST(initialize_escape_room(room, "company@mtm.com", 32, 80, 4,
                                       "12-21", 5) == MTM_SUCCESS);
    Order order = create_order();
    initialize_order(order, "client@mtm.com", ELECTRICAL_ENGINEERING, 57,
                     "0-12", 3);
    ASSERT_TEST(escape_room_add_order(room, order) == MTM_SUCCESS);
    ASSERT_TEST(company_add_room(company, room) == MTM_SUCCESS);
    ASSERT_TEST(company_remove_room(company, 2) == MTM_ID_DOES_NOT_EXIST);
    ASSERT_TEST(company_remove_room(company, 32) == MTM_RESERVATION_EXISTS);
    ASSERT_TEST(escape_room_remove_order(company_get_room(company, 32), order)
                == MTM_SUCCESS);
    ASSERT_TEST(company_remove_room(company, 32) == MTM_SUCCESS);
    order_remove(order);
    escape_room_destroy(room);
    company_destroy(company);
    return true;
}

static bool test_get_functions() {
    Company company = create_company();
    ASSERT_TEST(initialize_company(company, "company@mtm.com",
                                   BIOMEDICAL_ENGINEERING) == MTM_SUCCESS);
    ASSERT_TEST(company_get_room_num(company) == 0);
    ASSERT_TEST(company_get_faculty(company) == BIOMEDICAL_ENGINEERING);
    ASSERT_TEST(strcmp(company_get_email(company), "company@mtm.com") == 0);
    ASSERT_TEST(!company_got_orders(company));
    EscapeRoom room = create_escape_room();
    ASSERT_TEST(initialize_escape_room(room, "company@mtm.com", 32, 80, 4,
                                       "12-21", 5) == MTM_SUCCESS);
    Order order = create_order();
    ASSERT_TEST(initialize_order(order, "customer1@mtm.com",
                                 MECHANICAL_ENGINEERING, 32, "1-15", 3)
                == MTM_SUCCESS);
    ASSERT_TEST(escape_room_add_order(room, order) == MTM_SUCCESS);
    ASSERT_TEST(company_add_room(company, room) == MTM_SUCCESS);
    ASSERT_TEST(company_get_room_num(company) == 1);
    ASSERT_TEST(company_got_orders(company));
    order_remove(order);
    escape_room_destroy(room);
    company_destroy(company);
    return true;
}

static bool test_room_exists()
{
    Company company = create_company();
    ASSERT_TEST(initialize_company(company, "company@mtm.com",
                                   BIOMEDICAL_ENGINEERING) == MTM_SUCCESS);
    EscapeRoom room = create_escape_room();
    ASSERT_TEST(initialize_escape_room(room, "company@mtm.com", 32, 80, 4,
                                       "12-21", 5) == MTM_SUCCESS);
    ASSERT_TEST(!company_room_exists(NULL, 32));
    ASSERT_TEST(company_add_room(company, room) == MTM_SUCCESS);
    ASSERT_TEST(!company_room_exists(company, 30));
    ASSERT_TEST(company_room_exists(company, 32));
    ASSERT_TEST(company_remove_room(company, 32) == MTM_SUCCESS);
    ASSERT_TEST(!company_room_exists(company, 32));
    escape_room_destroy(room);
    company_destroy(company);
    return true;
}

static bool test_recommended_rooms()
{
    Company company = create_company();
    ASSERT_TEST(initialize_company(company, "company@mtm.com",
                                   BIOMEDICAL_ENGINEERING) == MTM_SUCCESS);
    EscapeRoom room1 = create_escape_room();
    ASSERT_TEST(initialize_escape_room(room1, "company@mtm.com", 32, 80, 4,
                                       "12-21", 3) == MTM_SUCCESS);
    EscapeRoom room2 = create_escape_room();
    ASSERT_TEST(initialize_escape_room(room2, "company@mtm.com", 33, 112, 6,
                                       "09-19", 2) == MTM_SUCCESS);
    EscapeRoom room3 =  create_escape_room();
    ASSERT_TEST(initialize_escape_room(room3, "company@mtm.com", 34, 112, 6,
                                       "18-23", 7) == MTM_SUCCESS);
    ASSERT_TEST(company_add_room(company, room1) == MTM_SUCCESS);
    ASSERT_TEST(company_add_room(company, room2) == MTM_SUCCESS);
    ASSERT_TEST(company_add_room(company, room3) == MTM_SUCCESS);
    Order order = create_order();
    initialize_order(order, "client@mtm.com", ELECTRICAL_ENGINEERING, 57,
                     "0-12", 3);
    ASSERT_TEST(escape_room_add_order(room1, order) == MTM_SUCCESS);
    int id, day, hour;

    /**
     * The calculation of the most recommended room is according to the given
     * formula.
     * Therefore, for a client ordering for 5 people with skill level 3,
     * the most recommended room will be room1 with the result of 1.
     * The following variables will have the next values:
     *  id - 32 (ID of room1)
     *  day - 0 (Reservations are available today)
     *  hour - 12 (A reservation for today at opening hour (11) exists.
     *         Next available hour is 12
     */
    ASSERT_TEST(company_recommended_rooms(company, 5, 3, &id, &day, &hour)==1);
    ASSERT_TEST(id == 32 && day == 0 && hour == 12);
    order_remove(order);
    escape_room_destroy(room1);
    escape_room_destroy(room2);
    escape_room_destroy(room3);
    company_destroy(company);
    return true;
}

static bool test_got_orders()
{
    ASSERT_TEST(!company_got_orders(NULL));
    Company company = create_company();
    ASSERT_TEST(initialize_company(company, "company@mtm.com",
                                   BIOMEDICAL_ENGINEERING) == MTM_SUCCESS);
    EscapeRoom room1 = create_escape_room();
    ASSERT_TEST(initialize_escape_room(room1, "company@mtm.com", 32, 80, 4,
                                       "12-21", 3) == MTM_SUCCESS);
    EscapeRoom room2 = create_escape_room();
    ASSERT_TEST(initialize_escape_room(room2, "company@mtm.com", 33, 112, 6,
                                       "09-19", 2) == MTM_SUCCESS);
    ASSERT_TEST(company_add_room(company, room1) == MTM_SUCCESS);
    ASSERT_TEST(company_add_room(company, room2) == MTM_SUCCESS);
    ASSERT_TEST(!company_got_orders(company));
    Order order = create_order();
    initialize_order(order, "client@mtm.com", ELECTRICAL_ENGINEERING, 57,
                     "0-12", 3);

    //Add order to room1 in the company
    ASSERT_TEST(escape_room_add_order(company_get_room(company, 32), order)
                == MTM_SUCCESS);
    ASSERT_TEST(company_got_orders(company));

    //Remove room1
    ASSERT_TEST(company_remove_room(company, 32) == MTM_RESERVATION_EXISTS);
    ASSERT_TEST(escape_room_remove_order(company_get_room(company, 32), order)
                == MTM_SUCCESS);
    ASSERT_TEST(!company_got_orders(company));
    order_remove(order);
    escape_room_destroy(room2);
    escape_room_destroy(room1);
    company_destroy(company);
    return true;
}

int main()
{
    RUN_TEST(test_create_company);
    RUN_TEST(test_initialize_and_copy);
    RUN_TEST(test_add_room);
    RUN_TEST(test_remove_room);
    RUN_TEST(test_get_functions);
    RUN_TEST(test_room_exists);
    RUN_TEST(test_recommended_rooms);
    RUN_TEST(test_got_orders);
    printf("\nAll tests passed!\n");
    printf("Hello\n");
    return 0;
}
