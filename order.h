#ifndef ORDER_H_
#define ORDER_H_

#include <stdio.h>
#include <stdbool.h>
#include "mtm_ex3.h"

typedef struct order *Order;

Order create_order();

//Creates a new order.
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

//Returns the email address of the order in the given memory space.
char* order_get_email(Order ord);

//Returns the faculty of the order in the given memory space.
TechnionFaculty order_get_faculty(Order ord);

//Returns the id of the order in the given memory space.
int order_get_id(Order ord);

//Returns the time of the order in the given memory space.
int order_get_day(Order ord);

int order_get_hour(Order ord);

bool order_compare_time(Order ord1, Order ord2);

bool orders_equal_time(Order ord1, Order ord2);

//Returns the number of peoples of the order in the given memory space.
unsigned int order_get_num_ppl(Order ord);

//More generic than set_new_time, this order reduces the days left until
//the order by 1.
void order_day_passed(Order ord);

//Destroys the order.
void order_remove(Order ord);

#endif
