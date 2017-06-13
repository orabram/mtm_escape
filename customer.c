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


static SetElement ord_copy(SetElement ord)
{
    Order ord2 = order_copy(ord);
    return ord2;

}

//This is the free function for the set.
static void ord_destroy(SetElement ord)
{
    order_remove(ord);
}

//This is the compare function for the set.
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

//Returns true if the email is legal, false otherwise.
static bool check_email(char* email)
{
    if(!strstr(email, "@"))
    {
        return false;
    }
    return true;
}

//Returns true if the faculty is legal, false otherwise.
static bool check_faculty(TechnionFaculty faculty)
{
    if(faculty >= FACULTIES_NUM || faculty < 0)
    {
        return false;
    }
    return true;
}

//Returns true if the skill is legal, false otherwise.
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
    cust->email = NULL;
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
    if(cust->OrderSet == NULL )
    {
        return MTM_OUT_OF_MEMORY;
    }
    if(cust->email != NULL)
    {
        free(cust->email);
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
    cust->OrderSet = setCreate(ord_copy, ord_destroy, ord_compare);
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

bool customer_already_booked(Order ord, Customer cust)
{
    return setIsIn(cust->OrderSet, ord);
}


void customer_destroy(Customer cust)
{
    if(cust->email != NULL)
    {
        free(cust->email);
        setDestroy(cust->OrderSet);
    }
    free(cust);
}
