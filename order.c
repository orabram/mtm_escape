#include "order.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"

#define FACULTIES_NUM 18

struct order{
    char* email;
    TechnionFaculty faculty;
    int id;
    int day;
    int hour;
    unsigned int num_ppl;
};

static int day_to_int(char* time)
{
    return (time[0] - '0')*10 + (time[1] - '0');
}

static int hour_to_int(char* time)
{
    return (time[3] - '0')*10 + (time[4] - '0');
}


static bool check_time(char* time)
{
    if(time == NULL || strlen(time) != 5 || time[2] != '-')
    {
        return false;
    }
    int day = day_to_int(time) , hour = hour_to_int(time);
    if(day < 0 || hour < 23 || hour > 23)
    {
        return false;
    }
    return true;
}

static bool check_email(char* email)
{
    if(email == NULL || !strstr(email, "@"))
    {
        return false;
    }
    return true;
}

static bool check_id(int id)
{
    if(id < 0)
    {
        return false;
    }
    return true;
}

static bool check_faculty(TechnionFaculty faculty)
{
    if(faculty >= FACULTIES_NUM || faculty < 0)
    {
        return false;
    }
    return true;
}

static bool check_num_ppl(int num_ppl)
{
    if(num_ppl <= 0)
    {
        return false;
    }
    return true;
}

Order create_order()
{
    Order ord = malloc(sizeof(Order));
    return ord;
}

MtmErrorCode initialize_order(Order ord, char* email, TechnionFaculty faculty,
int id, char* time, int num_ppl)
{
    if(ord == NULL || !check_time(time) || !check_email(email) || !check_id(id)
            || !check_faculty(faculty) || !check_num_ppl(num_ppl))
    {
        return MTM_INVALID_PARAMETER;
    }
    free(ord->email);
    ord->email = strdup(email);
    if(ord->email == NULL)
    {
        return MTM_OUT_OF_MEMORY;
    }
    ord->id = id;
    ord->faculty = faculty;
    ord->num_ppl = num_ppl;
    ord->day = day_to_int(time);
    ord->hour = hour_to_int(time);
    return MTM_SUCCESS;
}

char* order_get_email(Order ord)
{
    return ord->email;
}

//Returns the faculty of the order in the given memory space.
TechnionFaculty order_get_faculty(Order ord)
{
    return ord->faculty;
}

//Returns the id of the order in the given memory space.
int order_get_id(Order ord)
{
    return ord->id;
}

//Returns the time of the order in the given memory space.
int order_get_day(Order ord)
{
    return ord->day;
}

int order_get_hour(Order ord)
{
    return ord->hour;
}

bool order_compare_time(Order ord1, Order ord2)
{
    if(order_get_day(ord1)> order_get_day(ord2))
    {
        return true;
    }
    else if(order_get_day(ord1) == order_get_day(ord2))
    {
        if (order_get_hour(ord1) > order_get_day(ord2))
        {
            return true;
        }
    }
    return false;
}

bool order_equal_time(Order ord1, Order ord2)
{
    if(order_get_day(ord1) == order_get_day(ord2) && order_get_hour(ord1) ==
                                                     order_get_hour(ord2))
    {
        return true;
    }
    return false;
}

//Returns the number of peoples of the order in the given memory space.
unsigned int order_get_num_ppl(Order ord)
{
    return ord->num_ppl;
}

void order_day_passed(Order ord)
{
    ord->day -= 1;
}


//Destroys the order.
void order_remove(Order ord)
{
    free(ord->email);
    free(ord);
}


