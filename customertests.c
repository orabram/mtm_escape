#include <stdio.h>
#include <stdlib.h>
#include "order.h"
#include "customer.h"
#include <assert.h>
#include <string.h>


int main() {
    Order ord1 = create_order();
    Order ord2 = create_order();
    Order ord3;
    MtmErrorCode code;
    char email1[50] = "simon@gmail.com";
    TechnionFaculty faculty = CIVIL_ENGINEERING;
    int id = 12345;
    //int day = 0;
    //int hour = 23;
    char chrtime[50] = "00-23";
    unsigned int num_ppl = 3;
    char email2[50] =  "victan98@gmail.com";
    TechnionFaculty faculty2 = BIOMEDICAL_ENGINEERING;
    int id2 = 3;
    //int day2 = 3;
    //int hour2 = 5;
    char chrtime2[50] = "03-05";
    unsigned int num_ppl2 = 50;
    int skill1 = 5;
    int skill2 = 8;
    char faultymain[50] = "hillaryclinton";



    code = initialize_order(ord1, email1, faculty, id, chrtime, num_ppl);
    assert(code == MTM_SUCCESS);
    code = initialize_order(ord2, email2, faculty2, id2, chrtime2, num_ppl2);
    assert(code == MTM_SUCCESS);
    ord3 = order_copy(ord1);
    assert(orders_equal_time(ord1, ord3));
    Customer cust1 = create_customer();
    code = initialize_customer(cust1, email1, BIOLOGY, skill1);
    assert(code == MTM_SUCCESS);
    Customer cust2 = create_customer();
    code = initialize_customer(cust2, email2, AEROSPACE_ENGINEERING, skill2);
    assert(code == MTM_SUCCESS);
    code = initialize_customer(cust1, faultymain, BIOLOGY, skill1);
    assert(code == MTM_INVALID_PARAMETER);
    assert(!strcmp(email1, customer_get_email(cust1)));
    code = initialize_customer(cust1, 0, BIOLOGY, skill1);
    assert(code == MTM_NULL_PARAMETER);
    assert(!strcmp(email1, customer_get_email(cust1)));
    code = initialize_customer(cust1, email1, UNKNOWN, skill1);
    assert(code == MTM_INVALID_PARAMETER);
    assert(customer_get_faculty(cust1) == BIOLOGY);
    code = initialize_customer(cust1, email1, BIOLOGY, 0);
    assert(code == MTM_INVALID_PARAMETER);
    assert(customer_get_skill(cust1) == skill1);
    code = initialize_customer(cust1, email1, BIOLOGY, 23);
    assert(code == MTM_INVALID_PARAMETER);
    assert(customer_get_skill(cust1) == skill1);

    code = customer_add_order(ord1, cust1);
    assert(code == MTM_SUCCESS);
    assert(customer_get_orders_num(cust1) == 1);
    code = customer_add_order(ord2, cust1);
    assert(code == MTM_SUCCESS);
    assert(customer_get_orders_num(cust1) == 2);
    assert(customer_already_booked(ord2, cust1));
    assert(customer_add_order(ord3, cust1) == MTM_CLIENT_IN_ROOM);
    assert(customer_remove_order(ord2, cust1) == MTM_SUCCESS);
    assert(customer_get_orders_num(cust1) == 1);
    assert(orders_equal_time(customer_get_order(cust1, 0), ord1));
}