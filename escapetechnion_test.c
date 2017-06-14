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
    ASSERT_TEST(escapetechnion_add_room(escapeTechnion, "company@mtm.com", 13,
                                        88, 3, "20-24", 1) == MTM_SUCCESS);
    ASSERT_TEST(escapetechnion_remove_room(escapeTechnion, PHYSICS, 34)
                == MTM_ID_DOES_NOT_EXIST);
    ASSERT_TEST(escapetechnion_remove_room(escapeTechnion, CHEMISTRY, 33)
                == MTM_ID_DOES_NOT_EXIST);
    ASSERT_TEST(escapetechnion_remove_room(escapeTechnion, PHYSICS, 13)
                == MTM_SUCCESS);
    escapetechnion_destroy(escapeTechnion);
    return true;
}

static bool test_add_customer()
{
    EscapeTechnion escapeTechnion = create_escapetechnion();
    ASSERT_TEST(escapetechnion_add_customer(NULL, "customer@mtm.com", BIOLOGY,
                                            2) == MTM_NULL_PARAMETER);
    ASSERT_TEST(escapetechnion_add_customer(escapeTechnion, "customermtm.com",
                                            BIOLOGY,2)==MTM_INVALID_PARAMETER);
    ASSERT_TEST(escapetechnion_add_customer(escapeTechnion, "customer@mtm.com",
                                            BIOLOGY, 2) == MTM_SUCCESS);
    escapetechnion_destroy(escapeTechnion);
    return true;
}

static bool test_remove_customer()
{
    ASSERT_TEST(escapetechnion_remove_customer(NULL, "customer@mtm.com")
                == MTM_NULL_PARAMETER);
    EscapeTechnion escapeTechnion = create_escapetechnion();
    ASSERT_TEST(escapetechnion_add_customer(escapeTechnion, "customer@mtm.com",
                                            BIOLOGY, 2) == MTM_SUCCESS);
    ASSERT_TEST(escapetechnion_remove_customer(escapeTechnion
            , "customer2@mtm.com") == MTM_CLIENT_EMAIL_DOES_NOT_EXIST);
    ASSERT_TEST(escapetechnion_remove_customer(escapeTechnion
            , "customer@mtm.com") == MTM_SUCCESS);
    escapetechnion_destroy(escapeTechnion);
    return true;
}

static bool test_create_order()
{
    EscapeTechnion escapeTechnion = create_escapetechnion();
    ASSERT_TEST(escapetechnion_create_order(NULL, "customer@mtm.com"
            , CHEMISTRY, 45, "1-20", 2) == MTM_NULL_PARAMETER);
    ASSERT_TEST(escapetechnion_create_order(escapeTechnion, "customermtm.com"
            , CHEMISTRY, 45, "1-20", 2) == MTM_INVALID_PARAMETER);
    escapetechnion_add_company(escapeTechnion, "company@mtm.com", CHEMISTRY);
    escapetechnion_add_room(escapeTechnion, "company@mtm.com", 20, 92, 1,
                            "17-22", 1);
    escapetechnion_add_customer(escapeTechnion, "customer@mtm.com"
            , AEROSPACE_ENGINEERING, 2);
    ASSERT_TEST(escapetechnion_create_order(escapeTechnion, "customer2@mtm.com"
            , CHEMISTRY, 20, "1-20", 3) == MTM_CLIENT_EMAIL_DOES_NOT_EXIST);
    ASSERT_TEST(escapetechnion_create_order(escapeTechnion, "customer@mtm.com"
            , CHEMISTRY, 77, "1-20", 3) == MTM_ID_DOES_NOT_EXIST);
    ASSERT_TEST(escapetechnion_create_order(escapeTechnion, "customer@mtm.com"
            , CHEMISTRY, 20, "1-24", 3) == MTM_INVALID_PARAMETER);
    ASSERT_TEST(escapetechnion_create_order(escapeTechnion, "customer@mtm.com"
            , COMPUTER_SCIENCE, 20, "1-20", 3) == MTM_ID_DOES_NOT_EXIST);
    ASSERT_TEST(escapetechnion_create_order(escapeTechnion, "customer@mtm.com"
            , CHEMISTRY, 20, "1-20", 3) == MTM_SUCCESS);
    escapetechnion_destroy(escapeTechnion);
    return true;
}

static bool test_recommended_room()
{
    EscapeTechnion escapeTechnion = create_escapetechnion();
    ASSERT_TEST(escapetechnion_recommended_room(NULL, "customer@mtm.com", 5)
                == MTM_NULL_PARAMETER);
    escapetechnion_add_company(escapeTechnion, "company@mtm.com", CHEMISTRY);
    escapetechnion_add_room(escapeTechnion, "company@mtm.com", 20, 92, 1,
                            "17-22", 1);
    escapetechnion_add_room(escapeTechnion, "company@mtm.com", 22, 80, 3,
                            "02-12", 9);
    escapetechnion_add_customer(escapeTechnion, "customer@mtm.com",
                                AEROSPACE_ENGINEERING, 5);
    ASSERT_TEST(escapetechnion_recommended_room(NULL, "customer@mtm.com", 4)
                == MTM_NULL_PARAMETER);
    ASSERT_TEST(escapetechnion_recommended_room(escapeTechnion
            , "customer2@mtm.com", 4) == MTM_CLIENT_EMAIL_DOES_NOT_EXIST);
    ASSERT_TEST(escapetechnion_recommended_room(escapeTechnion
            , "customer@mtm.com", 4) == MTM_SUCCESS);
    escapetechnion_destroy(escapeTechnion);
    return true;
}

static bool test_report_day()
{
    EscapeTechnion escapeTechnion = create_escapetechnion();
    ASSERT_TEST(escapetechnion_reportday(NULL) == MTM_NULL_PARAMETER);
    ASSERT_TEST(escapetechnion_add_company(escapeTechnion, "company1@mtm.com",
                                           CHEMISTRY) == MTM_SUCCESS);
    ASSERT_TEST(escapetechnion_add_company(escapeTechnion, "company2@mtm.com"
            , COMPUTER_SCIENCE) == MTM_SUCCESS);
    ASSERT_TEST(escapetechnion_add_room(escapeTechnion, "company1@mtm.com", 20,
                                        92, 1, "17-22", 1) == MTM_SUCCESS);
    ASSERT_TEST(escapetechnion_add_room(escapeTechnion, "company1@mtm.com", 23,
                                        100, 8, "01-09", 2) == MTM_SUCCESS);
    ASSERT_TEST(escapetechnion_add_room(escapeTechnion, "company2@mtm.com", 22,
                                        80, 3, "02-12", 9) == MTM_SUCCESS);
    ASSERT_TEST(escapetechnion_add_customer(escapeTechnion, "customer1@mtm.com"
            , BIOTECHNOLOGY_AND_FOOD_ENGINEERING, 3) == MTM_SUCCESS);
    ASSERT_TEST(escapetechnion_add_customer(escapeTechnion, "customer2@mtm.com",
                                            CHEMICAL_ENGINEERING, 2) == MTM_SUCCESS);
    ASSERT_TEST(escapetechnion_create_order(escapeTechnion, "customer1@mtm.com"
            , CHEMISTRY, 20, "0-18", 12) == MTM_SUCCESS);
    ASSERT_TEST(escapetechnion_create_order(escapeTechnion, "customer1@mtm.com"
            , COMPUTER_SCIENCE, 22, "0-02", 1) == MTM_SUCCESS);
    ASSERT_TEST(escapetechnion_create_order(escapeTechnion, "customer2@mtm.com"
            , CHEMISTRY, 23, "1-08", 5) == MTM_SUCCESS);

    //A try to reserve orders at unavailable times(when a client exists in room)
    ASSERT_TEST(escapetechnion_create_order(escapeTechnion, "customer1@mtm.com"
            , CHEMISTRY, 23, "1-08", 2) == MTM_ROOM_NOT_AVAILABLE);
    ASSERT_TEST(escapetechnion_create_order(escapeTechnion, "customer1@mtm.com"
            , COMPUTER_SCIENCE, 22, "0-02", 6) == MTM_CLIENT_IN_ROOM);

    printf("\n---Daily Report Test:---\n");
    ASSERT_TEST(escapetechnion_reportday(escapeTechnion) == MTM_SUCCESS);

    //A try to reserve these orders again.
    //After a day passed, room No. 23 is taken by customer 2 at 08:00
    ASSERT_TEST(escapetechnion_create_order(escapeTechnion, "customer1@mtm.com"
            , CHEMISTRY, 23, "0-08", 2) == MTM_ROOM_NOT_AVAILABLE);
    //However room No. 22 is available today at 02:00 (after a day passed)
    ASSERT_TEST(escapetechnion_create_order(escapeTechnion, "customer1@mtm.com"
            , COMPUTER_SCIENCE, 22, "0-02", 6) == MTM_SUCCESS);
    printf("------------------------\n");
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
    RUN_TEST(test_add_customer);
    RUN_TEST(test_remove_customer);
    RUN_TEST(test_create_order);
    RUN_TEST(test_recommended_room);
    RUN_TEST(test_report_day);
    printf("\nAll tests passed!\n");
    return 0;
}