#ifndef ORDER_H_
#define ORDER_H_

#include <stdio.h>
#include <stdbool.h>
#include "mtm_ex3.h"

typedef struct order *Order;

Order create_order();

/*Initializes an order with the given parameters.
 * ord: an Order object.
 * email: The email of the customer. A char* with at least one @.
 * faculty: The faculty of the company the room belongs to.
 * Must be a legal faculty(must be lower than UNKNOWN).
 * id: The id of the room in the faculty. Must be an positive number.
 * time: The time until the order happens.
 * Must be written in a format of: "dd-hh", where dd marks the day and hh marks
 * the hours(must be lower than 23).
 * num_ppl: The number of people in the order. Must be a positive integer. */
MtmErrorCode initialize_order(Order ord, char* email, TechnionFaculty faculty,
                          int id, char* time, int num_ppl);
/*
//Copies an existing order
MtmErrorCode copy_order(Order ord, Order new_ord);

//Creates a new order, but with different parameters than the original one.
MtmErrorCode create_order_gen(Order ord, Customer cust, EscapeRoom escapy,
                              int time, unsigned int numppl);

//Changes the email address of the room.
//NOTE: It doesn't change the email of the customer that creates the order.
MtmErrorCode order_set_email(Order ord, char* email);

//Functions like set_email, but looks better in the code.
MtmErrorCode order_change_customer(Order ord, Customer cust);

//Changes the faculty of the room that was ordered.
//NOTE: It doesn't change the faculty of the original escape room.
MtmErrorCode order_set_faculty(Order ord, TechnionFaculty faculty);

//Changes the id of the room that was ordered.
//NOTE: It doesn't change the id of the original escape room.
MtmErrorCode order_set_id(Order ord, int id);

//Changes the room that was ordered(incorporates both set_id and set_faculty).
MtmErrorCode order_set_new_room(Order ord, EscapeRoom escapy);

//Changes the time that's left for the order.
MtmErrorCode order_set_new_time(Order ord, int time);

//Changes the number of people in the reservation.
MtmErrorCode order_set_numppl(Order ord, unsigned int num_ppl);
*/

//Returns the email address of the order.
char* order_get_email(Order ord);

//Returns the faculty of the order.
TechnionFaculty order_get_faculty(Order ord);

//Returns the id of the order.
int order_get_id(Order ord);

//Returns the day of the order.
int order_get_day(Order ord);

//Returns the hour of the order.
int order_get_hour(Order ord);

/*Compares between the dates of 2 orders.
 * Returns true if ord1's date is more distant than ord2's date.
 * Returns false otherwise.*/
bool order_compare_time(Order ord1, Order ord2);

/*Compares between the dates of 2 orders.
 * Returns true if ord1's date is equal to ord2's date.
 * Returns false otherwise*/
bool orders_equal_time(Order ord1, Order ord2);

//Returns the number of peoples of the order.
unsigned int order_get_num_ppl(Order ord);

/*Reduces the date of a given order by 1.
 * INPUT ASSUMPTION: day >=1. Won't do anything otherwise. */
void order_day_passed(Order ord);

//Destroys the order.
void order_remove(Order ord);

#endif
