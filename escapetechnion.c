
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
#include <math.h>
#include "set.h"


#define MAX_RECOMMENDED 9999
#define ILLEGAL_PRICE -1
#define FACULTIES_NUM 18

/**
 * Used in report_day function
 */
#define UPDATE_MIN() \
    cur_min = temp_min; \
    cur_faculty = temp_faculty; \
    min_room_id = temp_room_id; \


struct escapetechnion{
    Set CompanySet;
    Set CustomersSet;
    //Set CustomerEmailsSet;
    //Set CompanyEmailsSet;
    int orders_num;
    int faculties[FACULTIES_NUM];
    int days;
    FILE* output_channel;
};

/*This function receives an array of order objects and and EscapeTechnion object
 * and sorts the array according to the value and index of the items.*/

//This is the copy function for the set.
static SetElement comp_copy(SetElement comp)
{
    return company_copy(comp);
}

//This is the free function for the set.
static void comp_free(SetElement comp)
{
    company_destroy(comp);
}

//This is the compare function for the set.
static int comp_compare(SetElement comp1, SetElement comp2)
{
    return strcmp(company_get_email(comp1), company_get_email(comp2));
}

//This is the copy function for the set.
static SetElement cust_copy(SetElement cust)
{
    return customer_copy(cust);
}

//This is the free function for the set.
static void cust_free(SetElement cust)
{
    customer_destroy(cust);
}

//This is the compare function for the set.
static int cust_compare(SetElement cust1, SetElement cust2)
{
    return strcmp(customer_get_email(cust1), customer_get_email(cust2));
}


static char* time_int_to_chr(int day, int hour)
{
    char* chrtime = malloc(6);
    int digit1 = day / 10, digit2 = day % 10, digit3 = hour / 10,
            digit4 = hour % 10;
    if(chrtime == NULL)
    {
        return NULL;
    }
    chrtime[0] = digit1 + '0';
    chrtime[1] = digit2 + '0';
    chrtime[2] = '-';
    chrtime[3] = digit3 + '0';
    chrtime[4] = digit4 + '0';
    return chrtime;
}

/*Finds and returns an escaperoom object based on the id and faculty given.
 * Returns null if no match has been found.*/
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

//Will have to merge both find functions further fown the road.
static Customer find_customer_in_set(Set set, char* email)   /**TO MOVE **/
{
    Customer cust;
    char* temp_email;
    cust = setGetFirst(set);
    //Run through the set and look for a match.
    for(int i = 0; setGetSize(set); i++) {
        temp_email = customer_get_email(cust);
        //If the two emails are identical, that means we've got a match.
        if (!strcmp(temp_email, email)) {
            return cust;
        }
        cust = setGetNext(set);
    }
    return NULL;
}

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

static void order_sort(Order* sortedord, EscapeTechnion escape, int* prices)
{
    /**
     * Implemented as Bubble Sort
     */
    //Order swap;
    for (int i = 0 ; i < ( escape->orders_num - 1 ); i++)
    {
        for (int j = 0 ; j < escape->orders_num - i - 1; j++)
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
                    generic_swap(sortedord + j, sortedord + j + 1, sizeof(Order));
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

/*Runs through the Company Set and finds a Company object which matches the
 * email address*/
static Company find_company_in_set(Set set, char* email)
{
    Company comp;
    char* temp_email;
    comp = setGetFirst(set);
    //Run through the set and look for a match.
    for(int i = 0; setGetSize(set); i++) {
        temp_email = company_get_email(comp);
        //If the two emails are identical, that means we've got a match.
        if (!strcmp(temp_email, email)) {
            return comp;
        }
        comp = setGetNext(set);
    }
    return NULL;
}


/*Checks if the mail address is legal. Returns true or false according to
 * whether or not the email address is legal.*/
static bool check_email(char* email)
{
    if(email == NULL || !strstr(email, "@"))
    {
        return false;
    }
    return true;
}

/*Receives an Order and an EscapeTechnion object and calculates the price
 * of the order. Returns ILLEGAL_PRICE if no room was found.*/
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
        escape_room_get_difficulty(room), order_get_num_ppl(orders[i]),
                      prices[i]);
        order_remove(orders[i]);
    }
    mtmPrintDayFooter(escape->output_channel, escape->days);
}

/*This function prints all the relevant information to the daily business
 * according to mtm_ex3.h*/
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

static void reset_array(int* array, int len)
{
    for(int i = 0; i < len; i++)
    {
        array[i] = 0;
    }
}

/*This function doesn't receive parameters, and is responsible
 * for creating EscapeTechnion objects. */
EscapeTechnion create_escapetechnion(){
    EscapeTechnion escape = malloc(sizeof(EscapeTechnion));
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
        setClear(escape->CompanySet);
        free(escape);
        return NULL;
    }
    /*escape->CompanyEmailsSet = setCreate(strcpy, free , strcmp);
    if(escape->CompanyEmailsSet == NULL)
    {
        setClear(escape->CustomersSet);
        setClear(escape->CompanySet);
        free(escape);
        return NULL;
    }
    escape->CustomerEmailsSet = setCreate(strcpy, free, strcmp);
    if(escape->CustomerEmailsSet == NULL)
    {
        setClear(escape->CompanyEmailsSet);
        setClear(escape->CustomersSet);
        setClear(escape->CompanySet);
        free(escape);
        return NULL;
    }
    &(escape->orders_num) = malloc(sizeof(sizeof(int)));
    if(&(escape->orders_num) == NULL)
    {
        setClear(escape->CustomerEmailsSet);
        setClear(escape->CompanyEmailsSet);
        setClear(escape->CustomersSet);
        setClear(escape->CompanySet);
        free(escape);
    }*/
    reset_array(escape->faculties, FACULTIES_NUM);
    escape->days = 0;
    return escape;
}

MtmErrorCode escapetechnion_add_company(EscapeTechnion escape, char* email,
                                        TechnionFaculty faculty)
{
    Company newcomp = create_company();
    if(newcomp == NULL)
    {
        return MTM_OUT_OF_MEMORY;
    }
    if (find_customer_in_set(escape->CustomersSet, email) != NULL) {
        return MTM_EMAIL_ALREADY_EXISTS;
    }
    MtmErrorCode code = initialize_company(newcomp, email, faculty);
    if(code != MTM_SUCCESS)
    {
        company_destroy(newcomp);
        return code;
    }
    SetResult result = setAdd(escape->CompanySet, newcomp);
    switch (result) {
        case SET_OUT_OF_MEMORY:
            company_destroy(newcomp);
            return MTM_OUT_OF_MEMORY;
        case SET_ITEM_ALREADY_EXISTS:
            company_destroy(newcomp);
            return MTM_EMAIL_ALREADY_EXISTS;
        default:
            return MTM_SUCCESS;
    }
}

MtmErrorCode escapetechnion_destroy_company(EscapeTechnion escape, char* email)
{
    if(escape == NULL || check_email(email))
    {
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
    //setRemove(escape->CompanyEmailsSet, email);
    return MTM_SUCCESS;

}

MtmErrorCode escapetechnion_add_room(EscapeTechnion escape, char* email,
                                     int id, int price, int num_ppl,
                                     char* working_hrs, int difficulty)
{
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
    if(code != MTM_SUCCESS)
    {
        escape_room_destroy(room);
        return code;
    }
    return MTM_SUCCESS;
}

MtmErrorCode escapetechnion_remove_room(EscapeTechnion escape,
                                        TechnionFaculty faculty, int id)
{
    if(faculty >= UNKNOWN || id < 0)
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

MtmErrorCode escapetechnion_add_customer(EscapeTechnion escape, char* email,
                                         TechnionFaculty faculty,
                                         int skill_level)
{
    Customer cust = create_customer();
    if(cust == NULL)
    {
        return MTM_OUT_OF_MEMORY;
    }
    if (find_company_in_set(escape->CompanySet, email) != NULL) {
        return MTM_EMAIL_ALREADY_EXISTS;
    }
    MtmErrorCode code = initialize_customer(cust, email, faculty, skill_level);
    if(code != MTM_SUCCESS)
    {
        customer_destroy(cust);
        return code;
    }
    SetResult result = setAdd(escape->CustomersSet, cust);
    switch (result) {
        case SET_OUT_OF_MEMORY:
            customer_destroy(cust);
            return MTM_OUT_OF_MEMORY;
        case SET_ITEM_ALREADY_EXISTS:
            customer_destroy(cust);
            return MTM_EMAIL_ALREADY_EXISTS;
        default:
            return MTM_SUCCESS;
    }
}

MtmErrorCode escapetechnion_remove_customer(EscapeTechnion escape, char* email)
{
    if(escape == NULL || !check_email(email))
    {
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
    customer_destroy(cust);
    setRemove(escape->CustomersSet, email);
    //setRemove(escape->CustomerEmailsSet, email);
    return MTM_SUCCESS;
}

MtmErrorCode escapetechnion_create_order(EscapeTechnion escape, char* email,
                                         TechnionFaculty faculty, int id,
                                         char* time, int num_ppl)
{
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
    if(customer_already_booked(ord, cust))
    {
        order_remove(ord);
        return MTM_CLIENT_IN_ROOM;
    }
    code = escape_room_add_order(room, ord);
    if(code != MTM_SUCCESS)
    {
        order_remove(ord);
        return code;
    }
    code = customer_add_order(ord, cust);
    if(code != MTM_SUCCESS)
    {
        escape_room_remove_order(room, ord);
        order_remove(ord);
        return code;
    }
    escape->orders_num++;
    return MTM_SUCCESS;
}

MtmErrorCode escapetechnion_recommended_room(EscapeTechnion escape, char* email,
                                             int num_ppl)
{
    if(escape == NULL || !check_email(email) || num_ppl <= 0) {
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
            min_room_id, temp_room_id, day, hour;
    TechnionFaculty cur_faculty = UNKNOWN, temp_faculty,
            cust_faculty = customer_get_faculty(cust);
    for(int i = 0; i < setGetSize(escape->CompanySet); i++) {
        temp_min = company_recommended_rooms(comp, num_ppl, skill,
                                             &temp_room_id, &day, &hour);
        temp_faculty = company_get_faculty(comp);
        if(temp_min >= 0) {
            if (cur_min == -1) {
                UPDATE_MIN();
            }
            else if(temp_min < cur_min) {
                UPDATE_MIN();
            }
            else if(temp_min == cur_min) {
                if(fabs(temp_faculty - cust_faculty) <
                        fabs(cur_faculty - cust_faculty)) {
                    UPDATE_MIN();
                }
                else if(fabs(temp_faculty - cust_faculty) ==
                        fabs(cur_faculty - cur_faculty))  {
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
    char* chrtime = time_int_to_chr(day, hour);
    return escapetechnion_create_order(escape, email, cur_faculty, min_room_id,
                                       chrtime, num_ppl);
}

MtmErrorCode escapetechnion_reportday(EscapeTechnion escape)
{
    Customer cust = setGetFirst(escape->CustomersSet);
    TechnionFaculty faculty;
    Order* sortedord = malloc(sizeof(*sortedord) * (escape->orders_num));
    if(sortedord == NULL)
    {
        return MTM_OUT_OF_MEMORY;
    }
    int orders_num, counter = 0, price; //sum = 0;
    int* prices = malloc(escape->orders_num);
    Order ord;
    for(int i = 0; i < setGetSize(escape->CustomersSet); i++)
    {
        orders_num = customer_get_orders_num(cust);
        if(orders_num > 0)
        {
            for(int j = 0; j < orders_num; j++)
            {
                ord = customer_get_order(cust, j);
                if(order_get_day(ord) == 0)
                {
                    sortedord[counter] = ord;
                    counter++;
                    faculty = order_get_faculty(ord);
                    price = calculate_price(ord, escape);
                    if(faculty == customer_get_faculty(cust))
                    {
                        price *= 0.75;
                    }
                    escape->faculties[faculty] += price;
                    prices[counter] = price;
                    remove_order(ord, escape);
                    escape->orders_num--;
                }
                else
                {
                    order_day_passed(ord);
                }
            }
        }
        cust = setGetNext(escape->CustomersSet);
    }
    order_sort(sortedord, escape, prices);
    escape->days++;
    print_day(escape, counter, sortedord, prices);
    return MTM_SUCCESS;
}
void escapetechnion_reportbest(EscapeTechnion escape)
{
    int no1 = -1, no2 = -1, no3 = -1;
    int id1, id2, id3, temp, sum = 0;
    for(int i = 0; i < FACULTIES_NUM; i++)
    {
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
