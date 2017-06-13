#include "order.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FACULTIES_NUM UNKNOWN

struct order{
    char* email;
    TechnionFaculty faculty;
    int id;
    int day;
    int hour;
    unsigned int num_ppl;
};

//Extracts and returns the day from a given time string.
static int day_to_int(char* time)
{
    return (time[0] - '0')*10 + (time[1] - '0');
}

//Extracts and returns the hour from a given time string.
static int hour_to_int(char* time)
{
    return (time[3] - '0')*10 + (time[4] - '0');
}

//Returns true if the time string is legal, false otherwise.
static bool check_time(char* time)
{
    if(time == NULL || strlen(time) != 5 || time[2] != '-')
    {
        return false;
    }
    int day = day_to_int(time) , hour = hour_to_int(time);
    if(day < 0 || hour < 0 || hour > 23)
    {
        return false;
    }
    return true;
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

//Returns true if id is legal, false otherwise.
static bool check_id(int id)
{
    if(id < 0)
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

//Returns true if num_ppl is legal, false otherwise.
static bool check_num_ppl(int num_ppl)
{
    if(num_ppl <= 0)
    {
        return false;
    }
    return true;
}

//Creates and returns a new Order object.
Order create_order()
{
    Order ord = malloc(sizeof(struct order));
    return ord;
}

//Initializes an existing order with the given parameters.
MtmErrorCode initialize_order(Order ord, char* email, TechnionFaculty faculty,
int id, char* time, int num_ppl)
{
    //Check if the parameters are legal
    if(ord == NULL || email == NULL)
    {
        return MTM_NULL_PARAMETER;
    }
    if(!check_time(time) || !check_email(email) || !check_id(id)
       || !check_faculty(faculty) || !check_num_ppl(num_ppl))
    {
        return MTM_INVALID_PARAMETER;
    }
    //free(ord->email); //A precaution against initializing the same order twice.
    ord->email = strdup(email);
    if(ord->email == NULL)
    {
        return MTM_OUT_OF_MEMORY;
    }
    ord->id = id;
    ord->faculty = faculty;
    ord->num_ppl = (unsigned int)num_ppl;
    ord->day = day_to_int(time);
    ord->hour = hour_to_int(time); //Save the parameters in their respective
                                   //fields
    return MTM_SUCCESS;
}

//Creates a clone of an existing order and returns it.
Order order_copy(Order ord)
{
    Order new_ord = malloc(sizeof(struct order));
    if(new_ord == NULL)
    {
        return NULL;
    }
    new_ord->faculty = ord->faculty;
    new_ord->email = strdup(ord->email);
    new_ord->day = ord->day;
    new_ord->hour = ord->hour;
    new_ord->num_ppl = ord->num_ppl;
    new_ord->id = ord->id;
    return new_ord;
}

//Returns the email address of the client who made this order.
char* order_get_email(Order ord)
{
    return ord->email;
}

//Returns the faculty of the room the order was made to.
TechnionFaculty order_get_faculty(Order ord)
{
    return ord->faculty;
}

//Returns the id of the room the order was made to.
int order_get_id(Order ord)
{
    return ord->id;
}

//Returns the days until the order happens.
int order_get_day(Order ord)
{
    return ord->day;
}

//Returns the hours until the order happens.
int order_get_hour(Order ord)
{
    return ord->hour;
}

//Returns true if ord1 happens after ord2, false otherwise.
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

//Returns true if both orders happen in the same time, false otherwise.
bool orders_equal_time(Order ord1, Order ord2)
{
    if(order_get_day(ord1) == order_get_day(ord2) && order_get_hour(ord1) ==
                                                     order_get_hour(ord2))
    {
        return true;
    }
    return false;
}

//Returns the number of people in the order.
unsigned int order_get_num_ppl(Order ord)
{
    return ord->num_ppl;
}

//Deducts one from day if day>=1.
void order_day_passed(Order ord)
{
    if(ord->day != 0)
    {
        ord->day -= 1;
    }
}
//Destroys the order.
void order_remove(Order ord)
{
    if (ord == NULL) {
        return;
    }
    free(ord->email);
    free(ord);
}


