#include <stdio.h>
#include <stdlib.h>
#include "order.h"
#include <assert.h>
#include <string.h>

int main(){
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
    assert(code == MTM_SUCCESS);
    assert(day == 0);
    code = initialize_order(ord1, faultymail, faculty, id, chrtime, num_ppl);
    assert(code == MTM_INVALID_PARAMETER);
    assert(strcmp(order_get_email(ord1), email) == 0);
    code = initialize_order(ord1, email, UNKNOWN, id, chrtime, num_ppl);
    assert(code == MTM_INVALID_PARAMETER);
    assert(order_get_faculty(ord1) == faculty);
    code = initialize_order(ord1, email, faculty, -5, chrtime, num_ppl);
    assert(code == MTM_INVALID_PARAMETER);
    assert(order_get_id(ord1) == id);
    strcpy(chrtime, "0+-23");
    code = initialize_order(ord1, email, faculty, id, chrtime, num_ppl);
    assert(code == MTM_INVALID_PARAMETER);
    assert(order_get_day(ord1) == day);
    strcpy(chrtime, "00_23");
    code = initialize_order(ord1, email, faculty, id, chrtime, num_ppl);
    assert(code == MTM_INVALID_PARAMETER);
    assert(order_get_hour(ord1) == hour);
    strcpy(chrtime, "0024");
    code = initialize_order(ord1, email, faculty, id, chrtime, num_ppl);
    assert(code == MTM_INVALID_PARAMETER);
    assert(order_get_hour(ord1) == hour);
    strcpy(chrtime, "0023");
    code = initialize_order(ord1, email, faculty, id, chrtime, num_ppl);
    assert(code == MTM_INVALID_PARAMETER);
    assert(order_get_hour(ord1) == hour);
    code = initialize_order(ord1, email, faculty, id, chrtime, -1);
    assert(code == MTM_INVALID_PARAMETER);
    assert(order_get_num_ppl(ord1) == num_ppl);
    strcpy(chrtime, "01-23");
    code = initialize_order(ord1, email, faculty, id, chrtime, num_ppl);
    assert(code == MTM_SUCCESS);
    order_day_passed(ord1);
    assert(order_get_day(ord1) == day);
    order_remove(ord1);
    printf("All tests passed successfully. Congrats!");
}