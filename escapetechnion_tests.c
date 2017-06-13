#include <stdlib.h>
#include <stdio.h>
#include "escapetechnion.h"
#include "mtm_ex3.h"

#define ASSERT_TEST(b) do { \
        if (!(b)) { \
                printf("\nAssertion failed at %s:%d %s\n",__FILE__,__LINE__,#b); \
                return false; \
        } \
} while (0)


int main() {
    EscapeTechnion escape = create_escapetechnion();
    ASSERT_TEST(escape != NULL);
    MtmErrorCode code = escapetechnion_set_output_channel(escape, stdout);
    ASSERT_TEST(code == MTM_SUCCESS);
    code =escapetechnion_add_company(escape, "roomsinc@gmail.com", BIOLOGY);
    ASSERT_TEST(code == MTM_SUCCESS);
    escapetechnion_add_company(escape, "roomss", BIOLOGY);
    ASSERT_TEST(code == MTM_INVALID_PARAMETER);
    code = escapetechnion_add_company(escape, "roomsinc@gmail.com", BIOLOGY);
    ASSERT_TEST(code == MTM_EMAIL_ALREADY_EXISTS);
    code = escapetechnion_add_company(escape, "validmail@gmail.com", UNKNOWN);
    ASSERT_TEST(code == MTM_INVALID_PARAMETER);
    code = escapetechnion_add_company(escape, "validmail@gmail.com",
                                      AEROSPACE_ENGINEERING);
    ASSERT_TEST(code == MTM_SUCCESS);


/*
    company
    add
    roomsinc@gmail.com
    13
    room
    add
    roomsinc@gmail.com
    1
    100
    3
    06 - 18
    7
    escaper
    add
    escmaster@gmail.com
    13
    8

    escaper
    order
    escmaster@gmail.com
    13
    1
    0 - 10
    3
    report day

    escaper
    recommend
    escmaster@gmail.com
    3
    report day

    report best

    escaper
    remove
    escmaster@gmail.com
    room
    remove
    13
    1
    company
    remove
    roomsinc@gmail.com*/
}