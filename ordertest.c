#include <stdio.h>
#include "order.c"
#include <assert.h>

int main(){
    Order ord1 = create_order();
    Order ord2 = create_order();
    MtmErrorCode code;
    char* email = "simon@gmail.com";
    TechnionFaculty faculty = BIOLOGY;
    int id = 12345;
    int time = 0023;
    char* chrtime = "00-23";
    unsigned int num_ppl = 3;

    code = initialize_order(ord1, email, faculty, id, chrtime, num_ppl);
    assert(code == MTM_SUCCESS);
    assert(order_get_email(ord1) == email);
    code = initialize_order(ord1, "simon.gmail.com", faculty, id, chrtime, num_ppl);
    assert(code == MTM_INVALID_PARAMETER);
    assert(order_get_email(ord1) == email);
    code = initialize_order(ord1, email, UNKNOWN, id, chrtime, num_ppl);
    assert(code == MTM_INVALID_PARAMETER);
    assert(order_get_faculty(ord1) == faculty);
    code = initialize_order(ord1, email, faculty, -5, chrtime, num_ppl);
    assert(code == MTM_INVALID_PARAMETER);
    assert(order_get_id(ord1) == id);
    code = initialize_order(ord1, email, faculty, id, "+0-23", num_ppl);
    assert(code == MTM_INVALID_PARAMETER);
    assert(order_get_time(ord1) == time);
    code = initialize_order(ord1, email, faculty, id, "00-24", num_ppl);
    assert(code == MTM_INVALID_PARAMETER);
    assert(order_get_time(ord1) == time);
    code = initialize_order(ord1, email, faculty, id, "0024", num_ppl);
    assert(code == MTM_INVALID_PARAMETER);
    assert(order_get_time(ord1) == time);
    code = initialize_order(ord1, email, faculty, id, "00_24", num_ppl);
    assert(code == MTM_INVALID_PARAMETER);
    assert(order_get_time(ord1) == time);
    code = initialize_order(ord1, email, faculty, id, chrtime, -1);
    assert(code == MTM_INVALID_PARAMETER);
    assert(order_get_num_ppl(ord1) == num_ppl);
    code = initialize_order(ord1, email, faculty, id, "01-23", num_ppl);
    assert(code == MTM_SUCCESS);
    order_day_passed(ord1);
    assert(order_get_time(ord1) == time);
    printf("%s", "All tests passed successfully. Congrats!");
}