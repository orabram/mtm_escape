#include <stdio.h>
#include <stdlib.h>
#include "order.h"
#include <string.h>
#include "test_utilities.h"


static bool test_create_order()
{
    Order ord1 = create_order();
    ASSERT_TEST(ord1 !=NULL);
    order_remove(ord1);
    return true;
}


static bool test_initialize_order()
{
    Order ord1 = create_order();
    MtmErrorCode code;
    char email[100] = "simon@gmail.com";
    TechnionFaculty faculty = CIVIL_ENGINEERING;
    int id = 12345;
    int day = 0;
    int hour = 23;
    char chrtime[50] = "00-23";
    char faultymail[50] =  "simon.gmail.com";
    unsigned int num_ppl = 3;

    code = initialize_order(ord1, email, faculty, id, chrtime, num_ppl);
    ASSERT_TEST(code == MTM_SUCCESS);
    ASSERT_TEST(day == 0);
    code = initialize_order(ord1, faultymail, faculty, id, chrtime, num_ppl);
    ASSERT_TEST(code == MTM_INVALID_PARAMETER);
    ASSERT_TEST(strcmp(order_get_email(ord1), email) == 0);
    code = initialize_order(ord1, email, UNKNOWN, id, chrtime, num_ppl);
    ASSERT_TEST(code == MTM_INVALID_PARAMETER);
    ASSERT_TEST(order_get_faculty(ord1) == faculty);
    code = initialize_order(ord1, email, faculty, -5, chrtime, num_ppl);
    ASSERT_TEST(code == MTM_INVALID_PARAMETER);
    ASSERT_TEST(order_get_id(ord1) == id);
    strcpy(chrtime, "0+-23");
    code = initialize_order(ord1, email, faculty, id, chrtime, num_ppl);
    ASSERT_TEST(code == MTM_INVALID_PARAMETER);
    ASSERT_TEST(order_get_day(ord1) == day);
    strcpy(chrtime, "00_23");
    code = initialize_order(ord1, email, faculty, id, chrtime, num_ppl);
    ASSERT_TEST(code == MTM_INVALID_PARAMETER);
    ASSERT_TEST(order_get_hour(ord1) == hour);
    strcpy(chrtime, "00-24");
    code = initialize_order(ord1, email, faculty, id, chrtime, num_ppl);
    ASSERT_TEST(code == MTM_INVALID_PARAMETER);
    ASSERT_TEST(order_get_hour(ord1) == hour);
    strcpy(chrtime, "0023");
    code = initialize_order(ord1, email, faculty, id, chrtime, num_ppl);
    ASSERT_TEST(code == MTM_INVALID_PARAMETER);
    ASSERT_TEST(order_get_hour(ord1) == hour);
    code = initialize_order(ord1, email, faculty, id, chrtime, -1);
    ASSERT_TEST(code == MTM_INVALID_PARAMETER);
    ASSERT_TEST(order_get_num_ppl(ord1) == num_ppl);
    order_remove(ord1);
    return true;
}

static bool test_copy_order()
{
    Order ord1 = create_order();
    char email[100] = "simon@gmail.com";
    TechnionFaculty faculty = CIVIL_ENGINEERING;
    int id = 12345;
    char chrtime[50] = "00-23";
    ASSERT_TEST(initialize_order(ord1, email, faculty, id, chrtime, 3)
                == MTM_SUCCESS);
    Order ord2 = order_copy(ord1);
    ASSERT_TEST(ord2 != NULL);
    ASSERT_TEST(orders_equal_time(ord1, ord2));
    ASSERT_TEST(!strcmp(order_get_email(ord1), order_get_email(ord2)));
    ASSERT_TEST(order_get_num_ppl(ord1) == order_get_num_ppl(ord2));
    ASSERT_TEST(order_get_id(ord1) == order_get_id(ord2));
    ASSERT_TEST(order_get_faculty(ord1) == order_get_faculty(ord2));
    order_remove(ord1);
    order_remove(ord2);
    return true;
}

static bool test_time_functions()
{
    char chrtime[50] = "01-23";
    MtmErrorCode code;
    Order ord1 = create_order();
    char email[100] = "simon@gmail.com";
    TechnionFaculty faculty = CIVIL_ENGINEERING;
    int id = 12345;
    int num_ppl = 3;
    Order ord2 = create_order();
    char email2[100] = "shmuel@gmail.com";
    char chrtime2[50] = "02-23";
    TechnionFaculty faculty2 = AEROSPACE_ENGINEERING;
    int id2 = 58;

    code = initialize_order(ord1, email, faculty, id, chrtime, num_ppl);
    ASSERT_TEST(code == MTM_SUCCESS);
    code = initialize_order(ord2, email2, faculty2, id2, chrtime2, num_ppl + 1);
    ASSERT_TEST(code == MTM_SUCCESS);
    order_day_passed(ord1);
    ASSERT_TEST(order_get_day(ord1) == 0);
    ASSERT_TEST(!orders_equal_time(ord1, ord2));
    ASSERT_TEST(!order_compare_time(ord1, ord2));
    order_day_passed(ord2);
    order_day_passed(ord2);
    ASSERT_TEST(orders_equal_time(ord1, ord2));
    ASSERT_TEST(!order_compare_time(ord1, ord2));
    order_remove(ord1);
    order_remove(ord2);
    return true;

}

int main(){
    RUN_TEST(test_create_order);
    RUN_TEST(test_initialize_order);
    RUN_TEST(test_copy_order);
    RUN_TEST(test_time_functions);
    printf("All Order tests passed successfully. Congrats!");
}