#include <stdlib.h>
#include <stdio.h>
#include "escapetechnion.h"
#include "mtm_ex3.h"
#include "test_utilities.h"

static bool test_create_escapetechnion()
{
    EscapeTechnion escapeTechnion = create_escapetechnion();
    ASSERT_TEST(escapeTechnion != NULL);
    escapetechnion_destroy(escapeTechnion);
    return true;
}

static bool test_set_output_channel()
{
    EscapeTechnion escapeTechnion = create_escapetechnion();
    ASSERT_TEST(escapetechnion_set_output_channel(escapeTechnion, stdout) ==
                MTM_SUCCESS);
    escapetechnion_destroy(escapeTechnion);
    return true;
}

static bool test_add_and_remove_company()
{
    EscapeTechnion escapeTechnion = create_escapetechnion();
    ASSERT_TEST(escapetechnion_add_company(NULL, "company@mtm.com", PHYSICS) ==
                MTM_NULL_PARAMETER);
    ASSERT_TEST(escapetechnion_add_company(escapeTechnion, "companymtm.com",
                                           PHYSICS) == MTM_INVALID_PARAMETER);
    ASSERT_TEST(escapetechnion_add_company(escapeTechnion, "company@mtm.com",
                                           PHYSICS) == MTM_SUCCESS);
    ASSERT_TEST(escapetechnion_remove_company(NULL, "company@mtm.com")
                == MTM_NULL_PARAMETER);
    ASSERT_TEST(escapetechnion_remove_company(escapeTechnion,"company2@mtm.com")
                == MTM_COMPANY_EMAIL_DOES_NOT_EXIST);
    ASSERT_TEST(escapetechnion_remove_company(escapeTechnion, "company@mtm.com")
                == MTM_SUCCESS);
    escapetechnion_destroy(escapeTechnion);
    return true;
}

static bool test_add_room()
{
    EscapeTechnion escapeTechnion = create_escapetechnion();
    escapetechnion_add_company(escapeTechnion, "company@mtm.com", PHYSICS);
    ASSERT_TEST(escapetechnion_add_room(NULL, "company2@mtm.com", 22
            , 100, 2, "15-22", 4) == MTM_NULL_PARAMETER);
    ASSERT_TEST(escapetechnion_add_room(escapeTechnion, "company2@mtm.com", 22
            , 100, 2, "15-22", 4) == MTM_COMPANY_EMAIL_DOES_NOT_EXIST);
    ASSERT_TEST(escapetechnion_add_room(escapeTechnion, "company@mtm.com", 22
            , 101, 2, "15-22", 4) == MTM_INVALID_PARAMETER);
    ASSERT_TEST(escapetechnion_add_room(escapeTechnion, "company@mtm.com", 22
            , 100, 2, "15-22", 4) == MTM_SUCCESS);
    escapetechnion_destroy(escapeTechnion);
    return true;
}

static bool test_remove_room()
{
    EscapeTechnion escapeTechnion = create_escapetechnion();
    escapetechnion_add_company(escapeTechnion, "company@mtm.com", PHYSICS);
    ASSERT_TEST(escapetechnion_remove_room(NULL, PHYSICS, 33)
                == MTM_NULL_PARAMETER);
    ASSERT_TEST(escapetechnion_remove_room(escapeTechnion, PHYSICS, 34)
                == MTM_ID_DOES_NOT_EXIST);
    ASSERT_TEST(escapetechnion_remove_room(escapeTechnion, CHEMISTRY, 33)
                == MTM_ID_DOES_NOT_EXIST);
    ASSERT_TEST(escapetechnion_remove_room(escapeTechnion, PHYSICS, 33)
                == MTM_SUCCESS);
    escapetechnion_destroy(escapeTechnion);
    return true;
}


int main()
{
    RUN_TEST(test_create_escapetechnion);
    RUN_TEST(test_set_output_channel);
    RUN_TEST(test_add_and_remove_company);
    RUN_TEST(test_add_room);
    RUN_TEST(test_remove_room);
    printf("\nAll tests passed!\n");
    return 0;
}