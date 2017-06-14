#include <stdio.h>
#include <stdlib.h>
#include "order.h"
#include "customer.h"
#include <assert.h>
#include <string.h>
#include "test_utilities.h"


static bool test_create_customer()
{
    Customer cust = create_customer();
    ASSERT_TEST(cust != NULL);
    customer_destroy(cust);
    return true;
}

static bool test_initialize_company()
{
    MtmErrorCode code;
    char* email1 = "simon@gmail.com";
    char* email3 = "gideon@gmail.com";
    int skill1 = 5;
    char* faultymail = "hillaryclinton";

    Customer cust1 = create_customer();
    code = initialize_customer(cust1, email3, BIOLOGY, skill1);
    ASSERT_TEST(code == MTM_SUCCESS);
    Customer cust2 = create_customer();
    code = initialize_customer(cust2, faultymail, BIOLOGY, skill1);
    ASSERT_TEST(code == MTM_INVALID_PARAMETER);
    code = initialize_customer(cust2, 0, BIOLOGY, skill1);
    ASSERT_TEST(code == MTM_NULL_PARAMETER);
    code = initialize_customer(cust2, email1, UNKNOWN, skill1);
    ASSERT_TEST(code == MTM_INVALID_PARAMETER);
    code = initialize_customer(cust2, email1, BIOLOGY, 0);
    ASSERT_TEST(code == MTM_INVALID_PARAMETER);
    code = initialize_customer(cust2, email1, BIOLOGY, 23);
    ASSERT_TEST(code == MTM_INVALID_PARAMETER);
    customer_destroy(cust1);
    customer_destroy(cust2);
    return true;
}

static bool test_customer_order_management()
{
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
    char* email4 = "hillaryclinton@";


    code = initialize_order(ord1, email1, faculty, id, chrtime, num_ppl);
    ASSERT_TEST(code == MTM_SUCCESS);
    code = initialize_order(ord2, email2, faculty2, id2, chrtime2, num_ppl2);
    ASSERT_TEST(code == MTM_SUCCESS);
    ord3 = order_copy(ord1);
    ASSERT_TEST(orders_equal_time(ord1, ord3));
    Customer cust1 = create_customer();
    code = initialize_customer(cust1, email3, BIOLOGY, skill1);
    ASSERT_TEST(code == MTM_SUCCESS);
    Customer cust2 = create_customer();
    code = initialize_customer(cust2, email4, BIOLOGY, skill1);
    ASSERT_TEST(code == MTM_SUCCESS);
    code = customer_add_order(ord1, cust1);
    ASSERT_TEST(code == MTM_SUCCESS);
    ASSERT_TEST(customer_get_orders_num(cust1) == 1);
    code = customer_add_order(ord2, cust1);
    ASSERT_TEST(code == MTM_SUCCESS);
    ASSERT_TEST(customer_get_orders_num(cust1) == 2);
    ASSERT_TEST(customer_add_order(ord3, cust1) == MTM_CLIENT_IN_ROOM);
    ASSERT_TEST(customer_remove_order(ord2, cust1) == MTM_SUCCESS);
    ASSERT_TEST(customer_get_orders_num(cust1) == 1);
    ASSERT_TEST(orders_equal_time(customer_get_order(cust1, 0), ord1));
    order_remove(ord2);
    order_remove(ord1);
    order_remove(ord3);
    customer_destroy(cust1);
    customer_destroy(cust2);
    return true;
}

int main() {

    RUN_TEST(test_create_customer);
    RUN_TEST(test_initialize_company);
    RUN_TEST(test_customer_order_management);
    printf("You've passed all the tests. Congrats!");


}