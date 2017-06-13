#include <stdio.h>
#include <stdlib.h>
#include "order.h"
#include "customer.h"
#include <assert.h>
#include <string.h>

int ord_compare(Order ord1, Order ord2) {
    if (order_compare_time(ord1, ord2)) {
        return 1;
    }
    if (orders_equal_time(ord1, ord2)) {
        return 0;
    }
    return -1;
}
int main() {
    Order ord1 = create_order();
    Order ord2 = create_order();
    Order ord3;
    MtmErrorCode code;
    char* email1 = "simon@gmail.com";
    char* email3 = "gideon@gmail.com";
    TechnionFaculty faculty = CIVIL_ENGINEERING;
    int id = 12345;
    char* chrtime = "00-23";
    unsigned int num_ppl = 3;
    char* email2 =  "victan98@gmail.com";
    TechnionFaculty faculty2 = BIOMEDICAL_ENGINEERING;
    int id2 = 3;
    char* chrtime2 = "03-05";
    unsigned int num_ppl2 = 50;
    int skill1 = 5;
    char* faultymail = "hillaryclinton";

    code = initialize_order(ord1, email1, faculty, id, chrtime, num_ppl);
    assert(code == MTM_SUCCESS);
    code = initialize_order(ord2, email2, faculty2, id2, chrtime2, num_ppl2);
    assert(code == MTM_SUCCESS);
    ord3 = order_copy(ord1);
    assert(orders_equal_time(ord1, ord3));
    Customer cust1 = create_customer();
    code = initialize_customer(cust1, email3, BIOLOGY, skill1);
    assert(code == MTM_SUCCESS);
    Customer cust2 = create_customer();
    code = initialize_customer(cust2, faultymail, BIOLOGY, skill1);
    assert(code == MTM_INVALID_PARAMETER);
    code = initialize_customer(cust2, 0, BIOLOGY, skill1);
    assert(code == MTM_NULL_PARAMETER);
    code = initialize_customer(cust2, email1, UNKNOWN, skill1);
    assert(code == MTM_INVALID_PARAMETER);
    code = initialize_customer(cust2, email1, BIOLOGY, 0);
    assert(code == MTM_INVALID_PARAMETER);
    code = initialize_customer(cust2, email1, BIOLOGY, 23);
    assert(code == MTM_INVALID_PARAMETER);


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
    order_remove(ord2);
    order_remove(ord1);
    order_remove(ord3);
    customer_destroy(cust1);
    customer_destroy(cust2);
    printf("You've passed all the tests. Congrats!");


}