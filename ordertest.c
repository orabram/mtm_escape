#include <stdio.h>
#include <stdlib.h>
#include "order.h"
#include <assert.h>
#include <string.h>

int main(){
    Order ord1 = create_order();
    MtmErrorCode code;
    char* email = "simon@gmail.com";
    TechnionFaculty faculty = BIOLOGY;
    int id = 12345;
    int day = 0;
    int hour = 23;
    char* chrtime = "00-23";
    unsigned int num_ppl = 3;

    code = initialize_order(ord1, email, faculty, id, chrtime, num_ppl);
    chrtime = order_get_email(ord1);
    day = strcmp(chrtime, email);
    chrtime = "00-23";
    assert(code == MTM_SUCCESS);
    assert(day == 0);
    code = initialize_order(ord1, "simon.gmail.com", faculty, id, chrtime, num_ppl);
    assert(code == MTM_INVALID_PARAMETER);
    assert(strcmp(order_get_email(ord1), email) == 0);
    code = initialize_order(ord1, email, UNKNOWN, id, chrtime, num_ppl);
    assert(code == MTM_INVALID_PARAMETER);
    assert(order_get_faculty(ord1) == faculty);
    code = initialize_order(ord1, email, faculty, -5, chrtime, num_ppl);
    assert(code == MTM_INVALID_PARAMETER);
    assert(order_get_id(ord1) == id);
    code = initialize_order(ord1, email, faculty, id, "+0-23", num_ppl);
    assert(code == MTM_INVALID_PARAMETER);
    assert(order_get_day(ord1) == day);
    code = initialize_order(ord1, email, faculty, id, "00-24", num_ppl);
    assert(code == MTM_INVALID_PARAMETER);
    assert(order_get_hour(ord1) == hour);
    code = initialize_order(ord1, email, faculty, id, "0024", num_ppl);
    assert(code == MTM_INVALID_PARAMETER);
    assert(order_get_hour(ord1) == hour);
    code = initialize_order(ord1, email, faculty, id, "00_24", num_ppl);
    assert(code == MTM_INVALID_PARAMETER);
    assert(order_get_hour(ord1) == hour);
    code = initialize_order(ord1, email, faculty, id, chrtime, -1);
    assert(code == MTM_INVALID_PARAMETER);
    assert(order_get_num_ppl(ord1) == num_ppl);
    code = initialize_order(ord1, email, faculty, id, "01-23", num_ppl);
    assert(code == MTM_SUCCESS);
    order_day_passed(ord1);
    assert(order_get_day(ord1) == day);
    order_remove(ord1);
    printf("All tests passed successfully. Congrats!");
}