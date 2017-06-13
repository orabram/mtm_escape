#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <stdio.h>
#include <stdbool.h>
#include "mtm_ex3.h"
#include "set.h"
#include "order.h"
#include "escaperoom.h"

typedef struct customer *Customer;

/**
 * Allocates space for a new Customer object, and initializes its OrderSet.
 *
 * @return
 * NULL if there was a problem with the memory allocation for either Set or
 * Customer.
 * A Customer object otherwise.
 */
Customer create_customer();

/**
 * Initializes a newly created Customer object with the given values.
 *
 * @param cust: The Customer object.
 * @param email: The email of the new customer. Must be a char* with at least
 * one @.
 * @param faculty: The faculty of the new customer. Must be a positive integer
 * between 0 to 18.
 * @param skill: The skill level of the customer. A positive integer between
 * 1-10.
 * @return
 * MTM_OUT_OF_MEMORY if there was a problem allocating space for email.
 * MTM_NULL_PARAMETER if either cust or email are equal to NULL.
 * MTM_INVALID_PARAMETER if the given parameters don't meet their given
 * criteria.
 * MTM_SUCCESS otherwise.
 */
MtmErrorCode initialize_customer(Customer cust, char* email,
                             TechnionFaculty faculty, int skill);


/**
 * Receives an existing Customer object and clones it.
 *
 * @param cust: The Customer object we wish to clone.
 * @return
 * NULL if there was an allocation problem or if the given cust object is NULL.
 * A clone of cust otherwise.
 */
Customer customer_copy(Customer cust);

int customer_compare(Customer cust1, Customer cust2);


/**
 * Receives a Customer object and returns his email address.
 *
 * @param cust: The Customer whose email we wish to return.
 * @return
 * NULL if either cust or cust's email are equal to NULL.
 * An email address otherwise.
 */
char* customer_get_email(Customer cust);

/**
 * Receives a Customer object and returns his faculty.
 *
 * @param cust: The Customer whose faculty we wish to return.
 * @return
 * UNKNOWN if cust is equal to NULL.
 * The customer's faculty otherwise.
 */
TechnionFaculty customer_get_faculty(Customer cust);

/**
 * Receives a Customer object and returns his skill level.
 *
 * @param cust: The Customer whose skill level we wish to return.
 * @return
 * -1 if cust is equal to NULL.
 * The customer's faculty otherwise.
 */
int customer_get_skill(Customer cust);

/**
 *
 * Receives an order and adds it to the customer's OrderSet.
 *
 * @param ord: The order we wish to add.
 * @param cust: The customer we wish to add the order to.
 * @return
 * MTM_NULL_PARAMETER if either the customer or the order are NULL.
 * MTM_CLIENT_IN_ROOM if the client already has an order for this time.
 * MTM_OUT_OF_MEMORY if the Set has reached its limit.
 * MTM_SUCCESS otherwise.
 */
MtmErrorCode customer_add_order(Order ord, Customer cust);

/**
 * Receives a customer and an offset num, and returns an item from that offset.
 * NOTE: This function relies on the Set's internal iterator, and should only be
 * used when you can reliably know that it won't change.
 *
 * @example: int num_order = customer_get_num_order(cust);
 *           for(int i = 0; i < num_order; num++)
 *              order = customer_get_order(i);
 *              ...
 *
 * @param cust: The customer whose order we wish to return.
 * @param ord_num: The offset of the order from the beginning of the set.
 * @return
 * NULL if cust is equal to NULL or if ord_num is bigger than the amount of
 * orders the room has.
 * The requested order otherwise.
 */
Order customer_get_order(Customer cust, int ord_num);

/**
 *
 * Returns the number of order this customer has.
 *
 * @param cust: The customer whose orders_num we wish to return.
 * @return
 * -1 if cust is equal to NULL.
 * The number of orders otherwise.
 */
int customer_get_orders_num(Customer cust);

/**
 * Receives an order and a customer, and removes the order from the customer.
 *
 * @param ord: The order we wish to remove.
 * @param cust: The customer who has the order.
 * @return
 * MTM_NULL_PARAMETER if either cust or ord are equal to NULL.
 * MTM_INVALID_PARAMETER if the order isn't in the set.
 * MTM_SUCCESS otherwise.
 */
MtmErrorCode customer_remove_order(Order ord, Customer cust);


/**
 * Destroys a Customer object.
 *
 * @param cust: The customer we wish to destroy.
 */
void customer_destroy(Customer cust);
#endif