#ifndef ORDER_H_
#define ORDER_H_

#include <stdio.h>
#include <stdbool.h>
#include "mtm_ex3.h"

typedef struct order *Order;

Order create_order();

/**
 * This function initializes a newly created order with parameters.
 *
 * @param ord: The order we wish to initialize.
 * @param email: The email address of the customer who made the order. Must be
 * a char* and contain at least one @.
 * @param faculty: The faculty of the room we wish to visit. Must be a number
 * between 0 and UNKNOWN.
 * @param id: The id of the room we wish to order. Must be a positive integer.
 * @param time: The time until the order takes place. Must be a char* in
 * the format XX-YY, where XX is the number of days and YY is
 * the number of hours until the order takes place.
 * @param num_ppl: The number of people that the reservation is made to.
 * @return
 * MTM_OUT_OF_MEMORY: If a memory allocation problem happens.
 * MTM_NULL_PARAMETER: If ord, email or time are equal to NULL.
 * MTM_INVALID_PARAMETER: If one of the given parameters doesn't match his
 * criteria.
 * MTM_SUCCESS otherwise.
 */
MtmErrorCode initialize_order(Order ord, char* email, TechnionFaculty faculty,
                          int id, char* time, int num_ppl);

/**
 * Receives an existing order and copies it.
 *
 * @param ord: The order we wish to copy.
 * @return
 * NULL if ord is NULL or if the memory allocation failed.
 * A cloned Order otherwise.
 */
Order order_copy(Order ord);

/**
 * Receives an order and returns its email address.
 *
 * @param ord: The Order containing the email we wish to return.
 * @return
 * NULL if either ord or ord's email are equal to NULL.
 * An email address otherwise.
 */
char* order_get_email(Order ord);

/**
 * Receives an order and returns its faculty.
 *
 * @param ord: The Order containing the faculty we wish to return.
 * @return
 * UNKNOWN if ord is equal to NULL.
 * The faculty requested otherwise.
 */TechnionFaculty order_get_faculty(Order ord);

/**
 * Receives an order and returns its room id.
 *
 * @param ord: The Order containing the id we wish to return.
 * @return
 * -1 if ord is equal to ULL.
 * The order's room id otherwise.
 */int order_get_id(Order ord);

/**
 * Receives an order and returns how many days are left before the order takes
 * place.
 *
 * @param ord: The Order containing the day we wish to return.
 * @return
 * -1 if ord is equal to NULL
 * Order's day parameter otherwise.
 */int order_get_day(Order ord);

/**
 * Receives an order and returns the number of hours until the order happens.
 *
 * @param ord: The Order containing the hour we wish to return.
 * @return
 * NULL if is equal to NULL.
 * Order's hour parameter otherwise.
 */int order_get_hour(Order ord);

/**
 * Compares between the dates of 2 orders. At first it compares the day
 * parameter of those functions, and then it compares between their hours.
 *
 * @param ord1: The first order we compare.
 * @param ord2: The second order we compare.
 * @return
 * Returns true if ord1's date is more distant than ord2's date.
 * Returns false otherwise
 */
bool order_compare_time(Order ord1, Order ord2);

/**
 * Compares between the dates of 2 orders.
 * Returns true if ord1's date is equal to ord2's date. Same algorith as the
 * previous function, but this time it only checks for equality.
 * @param ord1: The first order we compare.
 * @param ord2: The second order we compare.
 * @return
 * Returns true if ord1's date is equal to ord2's date.
 * Returns false otherwise
 */
bool orders_equal_time(Order ord1, Order ord2);

/**
 * Receives an order and returns the number of people who'll attend the room.
 *
 * @param ord: The Order containing the hour we wish to return.
 * @return
 * 0 if order is equal to NULL.
 * Order's num_ppl parameter otherwise.
 */
 unsigned int order_get_num_ppl(Order ord);

/**
 * Deducts 1 from the day parameter of the order.
 * If day is zero(the order happens today) or ord is equal to NULL, nothing
 * will happen.
 * @param ord: The order whose hour is being deducted.
 */
void order_day_passed(Order ord);

/**
 * Destroys an order, freeing up all the memory.
 * @param ord: The order we're destroying.
 */
void order_remove(Order ord);

#endif
