#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "mtm_ex3.h"
#include <string.h>
#include "escapetechnion.h"
#include "company.h"
#include "customer.h"
#include "order.h"
#include "escaperoom.h"
#include "set.h"

#define ILLEGAL_PRICE -1
#define FACULTIES_NUM UNKNOWN

<<<<<<< HEAD

=======
>>>>>>> origin/master
struct escapetechnion{
    Set CompanySet;
    Set CustomersSet;
    int orders_num;
    int* faculties;
    int days;
    FILE* output_channel;
};

/**
 * Receives a SetElement and copies it.
 *
 * @param comp: the SetElement we wish to clone.
 * @return
 * The result of company_copy function.
 */
static SetElement comp_copy(SetElement comp)
{
    return company_copy(comp);
}

/**
 * Receives a SetElement and frees it.
 *
 * @param comp: The SetElement we wish to free.
 */
static void comp_free(SetElement comp)
{
    company_destroy(comp);
}

/**
 * Receives two SetElements and compares their e-mails.
 *
 * @param comp1: The first SetElement we wish to compare.
 * @param comp2: The second SetElement we wish to compare.
 * @return
 * Result of strcmp function applied on the e-mails of the elements.
 */
static int comp_compare(SetElement comp1, SetElement comp2)
{
    return strcmp(company_get_email(comp1), company_get_email(comp2));
}

/**
 * Receives a SetElement and copies it.
 *
 * @param cust: the SetElement we wish to clone.
 * @return
 * The result of customer_copy function.
 */
static SetElement cust_copy(SetElement cust)
{
    return customer_copy(cust);
}

/**
 * Receives a SetElement and frees it.
 *
 * @param comp: The SetElement we wish to free.
 */
static void cust_free(SetElement cust)
{
    customer_destroy(cust);
}

/**
 * Receives two SetElements and compares their e-mails.
 *
 * @param cust1: The first SetElement we wish to compare.
 * @param cust2: The second SetElement we wish to compare.
 * @return
 * Result of strcmp function applied on the e-mails of the elements.
 */
static int cust_compare(SetElement cust1, SetElement cust2)
{
    return strcmp(customer_get_email(cust1), customer_get_email(cust2));
}

/**
 * Updates all escape rooms in the system after a day passed.
 *
 * @param escape: The system
 * @return
 * MTM_NULL_PARAMETER if escape is NULL
 * MTM_SUCCESS otherwise
 */
static MtmErrorCode escapetechnion_day_passed(EscapeTechnion escape)
{
    if(escape == NULL)
    {
        return MTM_NULL_PARAMETER;
    }
    MtmErrorCode code;
    Company comp = setGetFirst(escape->CompanySet);
    for(int i = 0; i < setGetSize(escape->CompanySet); i++)
    {
        code = company_day_passed(comp);
        if(code != MTM_SUCCESS)
        {
            return code;
        }
        comp = setGetNext(escape->CompanySet);
    }
    return MTM_SUCCESS;
}

/**
<<<<<<< HEAD
=======
 * Converts the day and the hour, received as integers, to a string in
 * format "xx-yy", where "xx" is the day and "yy" is the hour.
 *
 * @param day: The day to convert
 * @param hour: The hour to convert
 * @return
 * The converted string
 */
/**
>>>>>>> origin/master
 * Returns the absolute value of a number.
 * @param x: an integer.
 * @return
 * Returns the absolute value of x;
 */
static int absolut(int x)
{
    if(x < 0)
    {
        return -x;
    }
    return x;
}

<<<<<<< HEAD
/**
 * Converts the day and the hour, received as integers, to a string in
 * format "xx-yy", where "xx" is the day and "yy" is the hour.
 *
 * @param day: The day to convert
 * @param hour: The hour to convert
 * @return
 * The converted string
 */
=======

>>>>>>> origin/master
static char* time_int_to_chr(int day, int hour)
{
    char* chrtime = malloc(6);
    if (day < 10) {
        sprintf(chrtime, "0%d-%d", day, hour);
    }
    else {
        sprintf(chrtime, "%d-%d", day, hour);
    }
    return chrtime;
}

/**
 * Finds the escape room using the faculty which it belongs to, and its ID.
 *
 * @param set: The set of escape rooms
 * @param id: The ID of the room
 * @param faculty: The faculty of the room
 * @return
 * NULL if the room not found
 * The room
 */
static EscapeRoom find_escape_room(Set set, int id, TechnionFaculty faculty)
{
    Company comp;
    comp = setGetFirst(set);
    EscapeRoom escape;
    for(int i = 0; i < setGetSize(set); i++)
    {
        if(company_get_faculty(comp) == faculty &&
           company_room_exists(comp, id))
        {
            escape = company_get_room(comp, id);
            return escape;
        }
        comp = setGetNext(set);
    }
    return NULL;
}

/**
 * Finds the customer in the customers' set using his e-mail
 *
 * @param set: The custimers' set
 * @param email: The customer's e-mail
 * @return
 * NULL if the customer not found
 * The customer
 */
static Customer find_customer_in_set(Set set, char* email)
{
    Customer cust;
    char* temp_email;
    cust = setGetFirst(set);

    //Run through the set and look for a match.
    for(int i = 0; i < setGetSize(set); i++) {
        temp_email = customer_get_email(cust);

        //If the two emails are identical, that means we've got a match.
        if (!strcmp(temp_email, email)) {
            return cust;
        }
        cust = setGetNext(set);
    }
    return NULL;
}

/**
 * Removes the given order from the system.
 *
 * @param ord: The order to remove
 * @param escape: The system
 * @return
 * MTM_NULL_PARAMETER if one of the parameters is NULL
 * MTM_INVALID_PARAMETER if the order not found in the system
 * MTM_SUCCESS otherwise
 */
static MtmErrorCode remove_order(Order ord, EscapeTechnion escape)
{
    EscapeRoom room = find_escape_room(escape->CompanySet, order_get_id(ord),
                                       order_get_faculty(ord));
    Customer cust = find_customer_in_set(escape->CustomersSet,
                                         order_get_email(ord));
    MtmErrorCode code = escape_room_remove_order(room, ord);
    if(code != MTM_SUCCESS)
    {
        return code;
    }
    code = customer_remove_order(ord, cust);
    if(code != MTM_SUCCESS)
    {
        return code;
    }
    return MTM_SUCCESS;
}
/**
 * Checks if there is at least one room in the system.
 *
 * @param escape: The system
 * @return
 * MTM_NO_ROOMS_AVAILABLE if there are no rooms in the system
 * MTM_SUCCESS if there is at least one room
 */
static MtmErrorCode rooms_in_system(EscapeTechnion escape)
{
    Company comp = setGetFirst(escape->CompanySet);
    for (int i = 0; i < setGetSize(escape->CompanySet); i++) {
        if(company_get_room_num(comp) > 0)
        {
            return MTM_SUCCESS;
        }
        comp = setGetNext(escape->CompanySet);
    }
    return MTM_NO_ROOMS_AVAILABLE;
}

/**
 * A generic function for swapping values between two parameters.
 *
 * @param x: Parameter 1
 * @param y: Parameter 2
 * @param size: The size of the elements in bytes
 *              (for example: if we want to swap two integers, then
 *               size = sizeof(int) )
 * @return
 * -1 if the swap was unsuccessful
 * 0 if the swap succeeded
 */
static int generic_swap(void *x, void *y, int size)
{
    void *tmp;
    if ((tmp = malloc(size)) == NULL) {
        return -1;
    }
    memcpy(tmp, x, size);
    memcpy(x, y, size);
    memcpy(y, tmp, size);
    free(tmp);
    return 0;
}

/**
 * Sorting function for an array of Orders and an array of Orders' prices.
 *
 * The primary criteria of sorting is the time of the order.
 * The secondary criteria is the ID of the faculty.
 * The last criteria (if there is still equality) is the order's room ID.
 *
 * The sorting of prices should conform to the sorting of the orders.
 *
 * The sorting is implemented using Bubble Sort algorithm.
 *
 * @param sortedord: The array of Orders to sort
 * @param escape: The system
 * @param prices: The array of Orders' prices
 * @param orders_num: Number of Orders and prices in their arrays.
 */
static void order_sort(Order* sortedord, EscapeTechnion escape, int* prices,
                       int orders_num)
{
    //Order swap;
    for (int i = 0 ; i < ( orders_num - 1 ); i++)
    {
        for (int j = 0 ; j < orders_num - i - 1; j++)
        {
            /*Check if the current order is scheduled before or after the next
             * order*/
            if (order_compare_time(sortedord[j], sortedord[j+1]))
            {
                generic_swap(sortedord + j, sortedord + j + 1, sizeof(Order));
                generic_swap(prices + j, prices + j + 1, sizeof(int));

            }
                /*If they're similar, check which one has a smaller faculty id*/
            else if(orders_equal_time(sortedord[j], sortedord[j+1]))
            {
                if(order_get_faculty(sortedord[j]) >
                   order_get_faculty(sortedord[j+1]))
                {
                    generic_swap(sortedord + j, sortedord + j+1, sizeof(Order));
                    generic_swap(prices + j, prices + j + 1, sizeof(int));
                }
                    /*If that's also equal, compare their id in their respective
                     * companies */
                else if(order_get_faculty(sortedord[j]) ==
                        order_get_faculty(sortedord[j+1]))
                {
                    if(order_get_id(sortedord[j]) >
                       order_get_id(sortedord[j+1]))
                    {
                        generic_swap(sortedord+j, sortedord+j+1, sizeof(Order));
                        generic_swap(prices+j, prices+j+1, sizeof(int));
                    }
                }
            }
        }
    }
}

/**
 * Finds the company in set of companies using the given e-mail of the company.
 *
 * @param set: Set of companies
 * @param email: The e-mail of the company
 * @return
 * NULL if company not found
 * The Company
 */
static Company find_company_in_set(Set set, char* email)
{
    Company comp;
    char* temp_email;
    comp = setGetFirst(set);
    //Run through the set and look for a match.
    for(int i = 0; i < setGetSize(set); i++) {
        temp_email = company_get_email(comp);
        //If the two emails are identical, that means we've got a match.
        if (!strcmp(temp_email, email)) {
            return comp;
        }
        comp = setGetNext(set);
    }
    return NULL;
}

/**
 * Checks if the e-mail is valid (contains exactly one '@' character)
 *
 * @param email: The e-mail to check
 * @return
 * True if it is valid
 * False otherwise
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
 * Receives an Order and an EscapeTechnion object and calculates the price
 * of the order.
 *
 * @param ord: The order to calculate its price
 * @param escape: The system
 * @return
 * ILLEGAL_PRICE if no room was found.
 * The price of the order
 */
static int calculate_price(Order ord, EscapeTechnion escape)
{
    EscapeRoom room;
    room = find_escape_room(escape->CompanySet, order_get_id(ord),
                            order_get_faculty(ord));
    if(room == NULL)
    {
        return ILLEGAL_PRICE;
    }
    return(escape_room_get_price(room) * order_get_num_ppl(ord));
}

/*This function prints all the relevant information to the daily business
 * according to mtm_ex3.h*/
/**
 * Prints all the relevant information to the daily business
 * according to mtm_ex3.h
 *
 * @param escape: The system
 * @param num_of_events: Number of events
 * @param orders: Array of orders
 * @param prices: Array of orders' prices
 */
static void print_day(EscapeTechnion escape, int num_of_events,
                      Order* orders, int* prices)
{
    EscapeRoom room;
    Customer cust;
    mtmPrintDayHeader(escape->output_channel, escape->days, num_of_events);
    for(int i = 0; i < num_of_events; i++)
    {
        room = find_escape_room(escape->CompanySet, order_get_id(orders[i]),
                                order_get_faculty(orders[i]));
        cust = find_customer_in_set(escape->CustomersSet,
                                    order_get_email(orders[i]));

        mtmPrintOrder(escape->output_channel, order_get_email(orders[i]),
                      customer_get_skill(cust), customer_get_faculty(cust),
                      escape_room_get_email(room), order_get_faculty(orders[i]),
                      order_get_id(orders[i]), order_get_hour(orders[i]),
                      escape_room_get_difficulty(room),
                      order_get_num_ppl(orders[i]), prices[i]);
        remove_order(orders[i], escape);
        escape->orders_num--;
    }
    mtmPrintDayFooter(escape->output_channel, escape->days);
}

/**
 * This function prints all the relevant information to the daily business
 * according to mtm_ex3.h
 *
 * @param escape: The system
 * @param sum: The total revenue
 * @param id1: ID of the 1st winner
 * @param no1: The revenue of the 1st winner
 * @param id2: ID of the 2nd winner
 * @param no2: The revenue of the 2nd winner
 * @param id3: ID of the 3rd winner
 * @param no3: The revenue of the 3rd winner
 */
static void print_winners(EscapeTechnion escape, int sum, int id1, int no1,
                          int id2, int no2, int id3, int no3)
{
    mtmPrintFacultiesHeader(escape->output_channel, FACULTIES_NUM, escape->days,
                            sum);
    mtmPrintFaculty(escape->output_channel, id1, no1);
    mtmPrintFaculty(escape->output_channel, id2, no2);
    mtmPrintFaculty(escape->output_channel, id3, no3);
    mtmPrintFacultiesFooter(escape->output_channel);
}

/**
 * Resets the array to contain 0 in each cell.
 *
 * @param array: The array to reset
 * @param len: Length of the array
 */
static void reset_array(int* array, int len)
{
    for(int i = 0; i < len; i++)
    {
        array[i] = 0;
    }
}

/*This function doesn't receive parameters, and is responsible
 * for creating EscapeTechnion objects. */
EscapeTechnion create_escapetechnion()
{
    EscapeTechnion escape = malloc(sizeof(*escape));
    if(escape == NULL)
    {
        return NULL;
    }
    escape->CompanySet = setCreate(comp_copy, comp_free, comp_compare);
    if (escape->CompanySet == NULL)
    {
        free(escape);
        return NULL;
    }
    escape->CustomersSet = setCreate(cust_copy, cust_free, cust_compare);
    if(escape->CustomersSet == NULL)
    {
        setDestroy(escape->CompanySet);
        free(escape);
        return NULL;
    }
    escape->faculties = malloc(FACULTIES_NUM * sizeof(int));
    reset_array(escape->faculties, FACULTIES_NUM);
    escape->days = 0;
    escape->orders_num = 0;
    escape->output_channel = stdout;
    return escape;
}

//Sets output channel for the system
MtmErrorCode escapetechnion_set_output_channel(EscapeTechnion escape,
                                               FILE* output_channel)
{
    if(escape == NULL || output_channel == NULL) {
        return MTM_NULL_PARAMETER;
    }
    escape->output_channel = output_channel;
    return MTM_SUCCESS;

}

//Adds company to the system
MtmErrorCode escapetechnion_add_company(EscapeTechnion escape, char* email,
                                        TechnionFaculty faculty)
{
    if (escape == NULL) {
        return MTM_NULL_PARAMETER;
    }
    Company newcomp = create_company();
    if(newcomp == NULL)
    {
        return MTM_OUT_OF_MEMORY;
    }
    if (find_customer_in_set(escape->CustomersSet, email) != NULL) {
        company_destroy(newcomp);
        return MTM_EMAIL_ALREADY_EXISTS;
    }
    MtmErrorCode code = initialize_company(newcomp, email, faculty);
    if(code != MTM_SUCCESS)
    {
        company_destroy(newcomp);
        return code;
    }
    SetResult result = setAdd(escape->CompanySet, newcomp);
    company_destroy(newcomp);
    switch (result) {
        case SET_OUT_OF_MEMORY:
            return MTM_OUT_OF_MEMORY;
        case SET_ITEM_ALREADY_EXISTS:
            return MTM_EMAIL_ALREADY_EXISTS;
        default:
            return MTM_SUCCESS;
    }
}

//Removes company from the system
MtmErrorCode escapetechnion_remove_company(EscapeTechnion escape, char* email)
{
    if(escape == NULL || email == NULL) {
        return MTM_NULL_PARAMETER;
    }
    if (!check_email(email)) {
        return MTM_INVALID_PARAMETER;
    }
    Company tempcomp = find_company_in_set(escape->CompanySet, email);
    if(tempcomp == NULL)
    {
        return MTM_COMPANY_EMAIL_DOES_NOT_EXIST;
    }
    if(company_got_orders(tempcomp))
    {
        return MTM_RESERVATION_EXISTS;
    }
    setRemove(escape->CompanySet, tempcomp);
    return MTM_SUCCESS;

}

//Adds room to the system
MtmErrorCode escapetechnion_add_room(EscapeTechnion escape, char* email,
                                     int id, int price, int num_ppl,
                                     char* working_hrs, int difficulty)
{
    if (escape == NULL) {
        return MTM_NULL_PARAMETER;
    }
    EscapeRoom room = create_escape_room();
    if(room == NULL)
    {
        return MTM_OUT_OF_MEMORY;
    }
    MtmErrorCode code = initialize_escape_room(room, email, id, price, num_ppl,
                                               working_hrs, difficulty);
    if(code != MTM_SUCCESS)
    {
        escape_room_destroy(room);
        return code;
    }
    Company comp = find_company_in_set(escape->CompanySet, email);
    if(comp == NULL)
    {
        escape_room_destroy(room);
        return MTM_COMPANY_EMAIL_DOES_NOT_EXIST;
    }
    TechnionFaculty faculty = company_get_faculty(comp);
    Company temp_comp = setGetFirst(escape->CompanySet);
    for(int i = 0; i < setGetSize(escape->CompanySet); i++)
    {
        if(company_get_faculty(temp_comp) == faculty &&
           company_room_exists(temp_comp, id))
        {
            escape_room_destroy(room);
            return MTM_ID_ALREADY_EXIST;
        }
        temp_comp = setGetNext(escape->CompanySet);
    }
    code = company_add_room(comp, room);
    escape_room_destroy(room);
    return code;
}

//Removes room from the system
MtmErrorCode escapetechnion_remove_room(EscapeTechnion escape,
                                        TechnionFaculty faculty, int id)
{
    if (escape == NULL) {
        return MTM_NULL_PARAMETER;
    }
    if(faculty >= UNKNOWN || id <= 0)
    {
        return MTM_INVALID_PARAMETER;
    }
    TechnionFaculty temp_faculty;
    MtmErrorCode result;
    Company comp = setGetFirst(escape->CompanySet);
    for(int i = 0; i < setGetSize(escape->CompanySet); i++)
    {
        temp_faculty = company_get_faculty(comp);
        if(faculty == temp_faculty)
        {
            assert(comp != NULL);
            result = company_remove_room(comp, id);
            if (result != MTM_ID_DOES_NOT_EXIST) {
                return result;
            }
        }
        comp = setGetNext(escape->CompanySet);
    }
    return MTM_ID_DOES_NOT_EXIST;
}

//Adds customer to the system
MtmErrorCode escapetechnion_add_customer(EscapeTechnion escape, char* email,
                                         TechnionFaculty faculty,
                                         int skill_level)
{
    if (escape == NULL) {
        return MTM_NULL_PARAMETER;
    }
    Customer cust = create_customer();
    if(cust == NULL)
    {
        return MTM_OUT_OF_MEMORY;
    }
    MtmErrorCode code = initialize_customer(cust, email, faculty, skill_level);
    if(code != MTM_SUCCESS)
    {
        customer_destroy(cust);
        return code;
    }
    if (find_company_in_set(escape->CompanySet, email) != NULL) {
        customer_destroy(cust);
        return MTM_EMAIL_ALREADY_EXISTS;
    }
    SetResult result = setAdd(escape->CustomersSet, cust);
    customer_destroy(cust);
    switch (result) {
        case SET_OUT_OF_MEMORY:
            return MTM_OUT_OF_MEMORY;
        case SET_ITEM_ALREADY_EXISTS:
            return MTM_EMAIL_ALREADY_EXISTS;
        default:
            return MTM_SUCCESS;
    }
}

//Removes customer from the system
MtmErrorCode escapetechnion_remove_customer(EscapeTechnion escape, char* email)
{
    if(escape == NULL || email == NULL) {
        return MTM_NULL_PARAMETER;
    }
    if (!check_email(email)) {
        return MTM_INVALID_PARAMETER;
    }
    Customer cust = find_customer_in_set(escape->CustomersSet, email);
    if (cust == NULL) {
        return  MTM_CLIENT_EMAIL_DOES_NOT_EXIST;
    }
    Order ord;
    for(int i = 0; i < customer_get_orders_num(cust); i++)
    {
        ord = customer_get_order(cust, i);
        remove_order(ord, escape);
    }
    escape->orders_num -= customer_get_orders_num(cust);
    setRemove(escape->CustomersSet, cust);
    //setRemove(escape->CustomerEmailsSet, email);
    return MTM_SUCCESS;
}

//Creates order of room for a customer in the system
MtmErrorCode escapetechnion_create_order(EscapeTechnion escape, char* email,
                                         TechnionFaculty faculty, int id,
                                         char* time, int num_ppl)
{
    if (escape == NULL) {
        return MTM_NULL_PARAMETER;
    }
    MtmErrorCode code;
    Order ord = create_order();
    if (ord == NULL) {
        return MTM_OUT_OF_MEMORY;
    }
    code = initialize_order(ord, email, faculty, id, time, num_ppl);
    if(code != MTM_SUCCESS)
    {
        order_remove(ord);
        return code;
    }
    Customer cust = find_customer_in_set(escape->CustomersSet, email);
    if(cust == NULL)
    {
        order_remove(ord);
        return MTM_CLIENT_EMAIL_DOES_NOT_EXIST;
    }
    EscapeRoom room = find_escape_room(escape->CompanySet, id, faculty);
    if(room == NULL)
    {
        order_remove(ord);
        return MTM_ID_DOES_NOT_EXIST;
    }
    code = customer_add_order(ord, cust);
    if(code != MTM_SUCCESS)
    {
        order_remove(ord);
        return code;
    }
    code = escape_room_add_order(room, ord);
    if(code != MTM_SUCCESS)
    {
        customer_remove_order(ord, cust);
        order_remove(ord);
        return code;
    }
    escape->orders_num++;
    order_remove(ord);
    return MTM_SUCCESS;
}

#define UPDATE_MIN() \
    cur_min = temp_min; \
    cur_faculty = temp_faculty; \
    min_room_id = temp_room_id; \
    cur_hour = hour; \
    cur_day = day; \


/* Finds the most recommended room for a customer and books it
 * as soon as possible
 */
MtmErrorCode escapetechnion_recommended_room(EscapeTechnion escape, char* email,
                                             int num_ppl)
{
    if(escape == NULL || email == NULL) {
        return MTM_NULL_PARAMETER;
    }
    if (!check_email(email) || num_ppl <= 0) {
        return MTM_INVALID_PARAMETER;
    }
    Customer cust = find_customer_in_set(escape->CustomersSet, email);
    if (cust == NULL) {
        return MTM_CLIENT_EMAIL_DOES_NOT_EXIST;
    }
    MtmErrorCode code = rooms_in_system(escape);
    if(code != MTM_SUCCESS) {
        return code;
    }
    Company comp = setGetFirst(escape->CompanySet);
    int cur_min = -1, temp_min, skill = customer_get_skill(cust),
            min_room_id = 0, temp_room_id = 0, day = 0, hour = 0, cur_day = 0,
            cur_hour = 0;
    TechnionFaculty cur_faculty = UNKNOWN, temp_faculty,
            cust_faculty = customer_get_faculty(cust);

    for(int i = 0; i < setGetSize(escape->CompanySet); i++) {
        temp_min = company_recommended_rooms(comp, num_ppl, skill,
                                             &temp_room_id, &day, &hour);
        temp_faculty = company_get_faculty(comp);

        //If The result is valid and first, update.
        if(temp_min >= 0) {
            if (cur_min == -1) {
                UPDATE_MIN();
            }
                //Sort by primary criteria
            else if(temp_min < cur_min) {
                UPDATE_MIN();
            }
                //Sort by secondary criteria
            else if(temp_min == cur_min) {
                if(absolut(temp_faculty - cust_faculty) <
                   absolut(cur_faculty - cust_faculty)) {
                    UPDATE_MIN();
                }
                    //Sort by the last criteria
                else if(absolut(temp_faculty - cust_faculty) ==
                        absolut(cur_faculty - cur_faculty))  {
                    if(temp_faculty < cur_faculty) {
                        UPDATE_MIN();
                    }
                    else if(temp_faculty == cur_faculty) {
                        if(temp_room_id < min_room_id) {
                            UPDATE_MIN();
                        }
                    }
                }
            }
        }
        comp = setGetNext(escape->CompanySet);
    }
    char* chrtime = time_int_to_chr(cur_day, cur_hour);
    MtmErrorCode res = escapetechnion_create_order(escape, email, cur_faculty
            , min_room_id, chrtime, num_ppl);
    free(chrtime);
    return res;
}

//Report the last day's orders and revenue
MtmErrorCode escapetechnion_reportday(EscapeTechnion escape)
{
    if (escape == NULL) {
        return MTM_NULL_PARAMETER;
    }
    Customer cust = setGetFirst(escape->CustomersSet);
    TechnionFaculty faculty;
    Order* sortedord = malloc(sizeof(*sortedord) * (escape->orders_num));
    if(sortedord == NULL) {
        return MTM_OUT_OF_MEMORY;
    }
    int orders_num, counter = 0, price;
    int* prices = malloc(sizeof(int) * escape->orders_num);
    Order ord;
    MtmErrorCode code;
    for(int i = 0; i < setGetSize(escape->CustomersSet); i++) {
        orders_num = customer_get_orders_num(cust);
        if(orders_num > 0) {
            for(int j = 0; j < orders_num; j++) {
                ord = customer_get_order(cust, j);

                //Make an array from orders for today
                if(order_get_day(ord) == 0)
                {
                    sortedord[counter] = ord;
                    faculty = order_get_faculty(ord);
                    price = calculate_price(ord, escape);
                    //Make a discount if the customer is at his faculty
                    if(faculty == customer_get_faculty(cust)) {
                        price *= 0.75;
                    }
                    escape->faculties[faculty] += price;
                    prices[counter] = price;
                    counter++;
                }
                else
                {
                    order_day_passed(ord);
                }
            }
        }
        cust = setGetNext(escape->CustomersSet);
    }
    code = escapetechnion_day_passed(escape);
    if(code != MTM_SUCCESS) {
        return code;
    }
    order_sort(sortedord, escape, prices, counter);

    //Print the sorted array of orders
    print_day(escape, counter, sortedord, prices);

    escape->days++;
    free(sortedord);
    free(prices);
    return MTM_SUCCESS;
}

//Report the most lucrative three faculties
void escapetechnion_reportbest(EscapeTechnion escape)
{
    int no1 = -1, no2 = -1, no3 = -1;
    int id1 = 0, id2 = 0, id3 = 0, temp = 0, sum = 0;
    for(int i = 0; i < FACULTIES_NUM; i++)
    {
        //Determine the first three places
        if(escape->faculties[i] > no1)
        {
            no3 = no2;
            no2 = no1;
            no1 = escape->faculties[i];
            id3 = id2;
            id2 = id1;
            id1 = i;
        }
        else if(escape->faculties[i] > no2)
        {
            no3 = no2;
            no2 = escape->faculties[i];
            id3 = id2;
            id2 = i;
        }
        else if(escape->faculties[i] > no3)
        {
            no3 = escape->faculties[i];
            id3 = i;
        }
        sum += escape->faculties[i];
    }
    if((no1 == no2) && (id1 > id2))
    {
        temp = id1;
        id1 = id2;
        id2 = temp;
    }
    if((no2 == no3) && (id2 > id3))
    {
        temp = id2;
        id2 = id3;
        id3 = temp;
    }
    print_winners(escape, sum, id1, no1, id2, no2, id3, no3);
}

//Destroys the system
void escapetechnion_destroy(EscapeTechnion escape)
{
    if (escape == NULL) {
        return;
    }
    setDestroy(escape->CustomersSet);
    setDestroy(escape->CompanySet);
    free(escape->faculties);
    free(escape);
}