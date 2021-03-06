#include "customer.h"
#include "order.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mtm_ex3.h"
#include "set.h"

#define FACULTIES_NUM UNKNOWN

struct customer {
    char *email;
    TechnionFaculty faculty;
    int skill_level;
    Set OrderSet;
    int orders_num;
};

/**
 * Receives a SetElement and copies it.
 *
 * @param ord: the SetElement we wish to clone.
 * @return
 * The result of order_copy(ord).
 */
static SetElement ord_copy(SetElement ord)
{
    Order ord2 = order_copy(ord);
    return ord2;

}

/**
 * Receives a SetElement and frees it.
 *
 * @param ord: The SetElement we wish to free.
 */
static void ord_destroy(SetElement ord)
{
    order_remove(ord);
}

/**
 * Receives two SetElements and compares their booking times.
 *
 * @param ord1: The first SetElement we wish to compare.
 * @param ord2: The second SetElement we wish to compare.
 * @return
 * If ord1 happens after ord2, it returns 1.
 * If ord1 happens at the same time as ord2, it returns 0.
 * If ord1 happens before ord2, it returns -1;
 */
static int ord_compare(SetElement ord1, SetElement ord2)
{
    if(order_compare_time(ord1, ord2))
    {
        return 1;
    }
    if(orders_equal_time(ord1, ord2))
    {
        return 0;
    }
    return -1;
}

/**
 * Receives an email address and checks if it is legal.
 *
 * @param email: The email address we check.
 * @return
 * false if it isn't legal(doesn't contain @).
 * true otherwise.
 */
static bool check_email(char* email)
{
    if(!strstr(email, "@")) //Checks if email contains at least 1 @.
    {
        return false;
    }
    if(strstr(strstr(email, "@") + 1, "@")) //Checks if email contains more
        // than one @.
    {
        return false;
    }
    return true;
}

/**
 * Receives a faculty and checks if it's legal(between 0 and unknown);
 *
 * @param faculty: The faculty we need to check.
 * @return
 * false if the value isn't legal.
 * true otherwise.
 */
static bool check_faculty(TechnionFaculty faculty)
{
    if(faculty >= FACULTIES_NUM || faculty < 0)
    {
        return false;
    }
    return true;
}

/**
 * Receives a skill and checks if it's legal(between 1 and 10);
 *
 * @param skill: The skill we need to check.
 * @return
 * false if the value isn't legal.
 * true otherwise.
 */
static bool check_skill(int skill)
{
    if(skill >= 1 && skill <= 10)
    {
        return true;
    }
    return false;
}

Customer create_customer()
{
    Customer cust = malloc(sizeof(struct customer));
    if(cust == NULL)
    {
        return NULL;
    }
    cust->OrderSet = setCreate(ord_copy, ord_destroy, ord_compare);
    if(cust->OrderSet == NULL)
    {
        free(cust);
        return NULL;
    }
    cust->email = "/";
    return cust;
}

MtmErrorCode initialize_customer(Customer cust, char* email,
                                 TechnionFaculty faculty, int skill)
{

    if(cust == NULL || email == NULL)
    {
        return MTM_NULL_PARAMETER;
    }
    if(!check_email(email) || !check_faculty(faculty) || !check_skill(skill))
    {
        return MTM_INVALID_PARAMETER;
    }
    cust->email = malloc(strlen(email) + 1);
    if(cust->email == NULL)
    {
        return MTM_OUT_OF_MEMORY;
    }
    strcpy(cust->email , email);
    cust->faculty = faculty;
    cust->orders_num = 0;
    cust->skill_level = skill;
    return MTM_SUCCESS;

}

Customer customer_copy(Customer cust)
{
    if(cust == NULL)
    {
        return NULL;
    }
    Customer new_cust = malloc(sizeof(struct customer));
    if(new_cust == NULL)
    {
        return NULL;
    }
    new_cust->OrderSet = setCopy(cust->OrderSet);
    new_cust->email = malloc(strlen(cust->email) + 1);
    if(new_cust->email == NULL)
    {
        free(new_cust);
        return NULL;
    }
    strcpy(new_cust->email, cust->email);
    new_cust->faculty = cust->faculty;
    new_cust->skill_level = cust->skill_level;
    new_cust->orders_num = cust->orders_num;
    return new_cust;
}

//Returns the email address of the customer.
char* customer_get_email(Customer cust)
{
    return cust->email;
}

//Returns the faculty of the customer.
TechnionFaculty customer_get_faculty(Customer cust)
{
    return cust->faculty;
}

//Returns the skill level of the customer.
int customer_get_skill(Customer cust)
{
    return cust->skill_level;
}

MtmErrorCode customer_add_order(Order ord, Customer cust)
{
    if(ord == NULL || cust == NULL)
    {
        return MTM_NULL_PARAMETER;
    }
    SetResult result = setAdd(cust->OrderSet, ord);
    if(result == SET_ITEM_ALREADY_EXISTS)
    {
        return MTM_CLIENT_IN_ROOM;
    }
    if (result == SET_OUT_OF_MEMORY)
    {
        return MTM_OUT_OF_MEMORY;
    }
    cust->orders_num++;
    return MTM_SUCCESS;
}

int customer_get_orders_num(Customer cust)
{
    return cust->orders_num;
}

Order customer_get_order(Customer cust, int ord_num)
{
    if(ord_num > setGetSize(cust->OrderSet))
    {
        return NULL;
    }
    Order ord = setGetFirst(cust->OrderSet);
    for(int i = 0; i < ord_num; i++)
    {
        ord = setGetNext(cust->OrderSet);
    }
    return ord;
}

MtmErrorCode customer_remove_order(Order ord, Customer cust)
{
    SetResult result = setRemove(cust->OrderSet, ord);
    if(result != SET_SUCCESS)
    {
        return MTM_INVALID_PARAMETER;
    }
    cust->orders_num--;
    return MTM_SUCCESS;

}

void customer_destroy(Customer cust)
{
    if (cust == NULL) {
        return;
    }
    if(strcmp(cust->email, "/") != 0) {
        free(cust->email);
    }
    setDestroy(cust->OrderSet);
    free(cust);
}
