#ifndef CUSTUMER_H_
#define CUSTUMER_H_

#include <stdio.h>
#include <stdbool.h>
#include "mtm_ex3.h"
#include "set.h"
#include "order.h"

typedef struct customer{
    char* email;
    TechnionFaculty faculty;
    int skill_level;
    Set OrderSet;

} *Customer;

//Creates and returns a new instance of customer. Resets OrderSet.
MtmErrorCode create_customer(Customer cust, char* email,
                             TechnionFaculty faculty, int skill);

//Receives a costumer and returns a copy of the original.
MtmErrorCode customer_copy(Customer original_cust, Customer new_cust);

//Receives a costumer and a new email address and updates it.
//Will also update the email in every order that the customer currently has.
MtmErrorCode customer_set_email(Customer cust, char* newmail);

//Receives a costumer and a new faculty and updates it.
MtmErrorCode customer_set_faculty(Customer cust, TechnionFaculty newfac);

//Receives a costumer and a new skill level and updates it.
MtmErrorCode customer_set_skill(Customer cust, int skill);

//Returns the email address of the customer.
MtmErrorCode customer_get_email(Customer cust, char* email);

//Returns the faculty of the customer.
MtmErrorCode customer_get_faculty(Customer cust, TechnionFaculty* faculty);

//Returns the skill level of the customer.
MtmErrorCode customer_get_skill(Customer cust, int* skill);

//Sets true if the email address is used by the customer.
MtmErrorCode customer_already_exists(Customer cust, char* email, bool* correct);

//Adds a new order to the order set.
MtmErrorCode customer_new_order(Order ord, Customer cust);

//Checks if the order already exists.
MtmErrorCode customer_order_exists(Order ord, Customer cust, bool* correct);

//Checks if the customer has any other orders at the time of the order.
MtmErrorCode customer_already_booked(Order ord, Customer cust, bool* correct);

//Removes an order from the set.
MtmErrorCode customer_remove_order(Order ord, Customer cust);

//Checks if the customers has any impending orders.
MtmErrorCode customer_has_orders(Order ord, Customer cust, bool* correct);

//Destroys the customer.
MtmErrorCode customer_destroy(Customer cust);
#endif