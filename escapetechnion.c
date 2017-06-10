#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "string.h"
#include "escapetechnion.h"
#include <math.h>


#define MAX_RECOMMENDED 9999
#define ILLEGAL_PRICE -1
#define FACULTIES_NUM 18


struct escapetechnion{
    Set CompanySet;
    Set CustomersSet;
    Set CustomerEmailsSet;
    Set CompanyEmailsSet;
    int orders_num;
    int faculties[FACULTIES_NUM] = {0};
    int days;
    FILE* output_channel;
};

/*This function receives an array of order objects and and EscapeTechnion object
 * and sorts the array according to the value and index of the items.*/
static void order_bubble_sort(Order* sortedord, EscapeTechnion escape)
{
    Order swap;
    for (int i = 0 ; i < ( escape->orders_num - 1 ); i++)
    {
        for (int j = 0 ; j < escape->orders_num - i - 1; j++)
        {
           /*Check if the current order is scheduled before or after the next
            * order*/
            if (order_get_time(sortedord[i] > order_get_time((sortedord[i+1]))))
            {
                swap = sortedord[j];
                sortedord[j] = sortedord[j+1];
                sortedord[j+1] = swap;
            }
            /*If they're similar, check which one has a smaller faculty id*/
            else if(order_get_time(sortedord[i] ==
                                   order_get_time((sortedord[i+1]))))
            {
                if(order_get_faculty(sortedord[i]) >
                        order_get_faculty(sortedord[i+1]))
                {
                    swap = sortedord[j];
                    sortedord[j] = sortedord[j+1];
                    sortedord[j+1] = swap;
                }
                /*If that's also equal, compare their id in their respective
                 * companies */
                else if(order_get_faculty(sortedord[i]) ==
                        order_get_faculty(sortedord[i+1]))
                {
                    if(order_get_id(sortedord[i]) >
                            order_get_id(sortedord[i+1]))
                    {
                        swap = sortedord[j];
                        sortedord[j] = sortedord[j+1];
                        sortedord[j+1] = swap;
                    }
                }
            }
        }
    }
}

/*Runs through the Company Set and finds a Company object which matches the
 * email address*/
static MtmErrorCode find_company_in_set(Set set, char* email, Company comp)
{
    char** temp_email;
    comp = setGetFirst(set);
    //Run through the set and look for a match.
    for(int i = 0; setGetSize(set); i++) {
        temp_email = company_get_email(comp);
        if (temp_email == NULL) {
            return MTM_OUT_OF_MEMORY;
        }
        //If the two emails are identical, that means we've got a match.
        if (!strcmp(*temp_email, email)) {
            return MTM_SUCCESS;
        }
        comp = setGetNext(set);
    }
}

//Will have to merge both find functions further fown the road.
static MtmErrorCode find_customer_in_set(Set set, char* email, Customer cust)
{
    char** temp_email;
    cust = setGetFirst(set);
    //Run through the set and look for a match.
    for(int i = 0; setGetSize(set); i++) {
        temp_email = company_get_email(cust);
        if (temp_email == NULL) {
            return MTM_OUT_OF_MEMORY;
        }
        //If the two emails are identical, that means we've got a match.
        if (!strcmp(*temp_email, email)) {
            return MTM_SUCCESS;
        }
        cust = setGetNext(set);
    }
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
static void print_day(EscapeTechnion escape, int num_of_events, Order* orders, int sum)
{
    EscapeRoom room;
    Customer cust;
    MtmErrorCode code;
    mtmPrintDayHeader(escape->output_channel, escape->days, num_of_events);
    for(int i = 0; i < num_of_events; i++)
    {
        room = find_escape_room(escape->CompanySet, order_get_id(orders[i]),
                                order_get_faculty(orders[i]));
        code = find_customer_in_set(escape->CustomersSet,
                                    order_get_email(orders[i]), cust);
        mtmPrintOrder(escape->output_channel, order_get_email(orders[i]),
        customer_get_skill(cust), customer_get_faculty(cust),
        escape_room_get_email(orders[i]), order_get_faculty(orders[i]),
        order_get_id(orders[i]), order_get_hour(orders[i]),
        escape_room_get_difficulty(orders[i]), order_get_num_ppl(orders[i]), sum);
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

/*This function doesn't receive parameters, and is responsible
 * for creating EscapeTechnion objects. */
EscapeTechnion create_escapetechnion(){
    EscapeTechnion escape = malloc(sizeof(EscapeTechnion));
    if(escape == NULL)
    {
        return NULL;
    }
    escape->CompanySet = setCreate(company_copy, company_destroy, company_compare);
    if (escape->CompanySet == NULL)
    {
        free(escape);
        return NULL;
    }
    escape->CustomersSet = setCreate(customer_copy, customer_destroy, customer_compare);
    if(escape->CustomersSet == NULL)
    {
        setClear(escape->CompanySet);
        free(escape);
        return NULL;
    }
    escape->CompanyEmailsSet = setCreate(strcpy, free , strcmp);
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
    }
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
    MtmErrorCode code = initialize_company(newcomp, email, faculty);
    if(code != MTM_SUCCESS)
    {
        company_destroy(newcomp);
        return code;
    }
    setAdd(escape->CompanySet, newcomp);
    setAdd(escape->CompanyEmailsSet, email);
    return MTM_SUCCESS;
}

MtmErrorCode escapetechnion_destroy_company(EscapeTechnion escape, char* email)
{
    if(escape == NULL || check_email(email))
    {
        return MTM_INVALID_PARAMETER;
    }
    if(!setIsIn(escape->CompanyEmailsSet, email))
    {
        return MTM_COMPANY_EMAIL_DOES_NOT_EXIST;
    }
    MtmErrorCode code;
    Company tempcomp;
    MtmErrorCode code;
    char** temp_email;
    bool has_orders;
    code = find_company_in_set(escape->CompanySet, email, tempcomp);
    if(code != MTM_SUCCESS)
    {
        return code;
    }
    code = company_got_orders(tempcomp, &has_orders);
    if(code != MTM_SUCCESS)
    {
        return code;
    }
    if(!has_orders)
    {
        return MTM_RESERVATION_EXISTS;
    }
    setRemove(escape->CompanySet, email);
    setRemove(escape->CompanyEmailsSet, email);
    company_destroy(tempcomp);
    return MTM_SUCCESS;

}

MtmErrorCode escapetechnion_add_room(EscapeTechnion escape, char* email,
                                     int id, int price, int num_ppl,
                                     char* working_hrs, int difficulty)
{
    EscapeRoom room = create_escape_room();
    bool room_already_exists;
    if(room == NULL)
    {
        return MTM_OUT_OF_MEMORY;
    }
    MtmErrorCode code;
    code = initialize_escape_room(email, id, price, num_ppl, working_hrs,
                                  difficulty);
    if(code != MTM_SUCCESS)
    {
        escape_room_destroy(room);
        return code;
    }
    if(!setIsIn(escape->CompanyEmailsSet, email))
    {
        escape_room_destroy(room);
        return MTM_COMPANY_EMAIL_DOES_NOT_EXIST;
    }
    Company tempcomp; //Could potentially cause troubles.
    code = find_company_in_set(escape->CompanySet, email, tempcomp);
    if(code != MTM_SUCCESS)
    {
        escape_room_destroy(room);
        return code;
    }
    code = company_contains(tempcomp, id, room_already_exists);
    if(code != MTM_SUCCESS)
    {
        escape_room_destroy(room);
        return code;
    }
    if(room_already_exists)
    {
        escape_room_destroy(room);
        return MTM_ID_ALREADY_EXIST;
    }
    code = company_add_room(tempcomp, room);
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
    if(faculty >= UNKNOWN)
    {
        return MTM_INVALID_PARAMETER;
    }
    Company tempcomp;
    TechnionFaculty temp_faculty;
    char* temp_email;
    bool room_has_orders;
    MtmErrorCode code;
    tempcomp = setGetFirst(escape->CompanySet);
    for(int i = 0; i < setGetSize(escape->CompanySet); i++)
    {
        temp_faculty = company_get_faculty(tempcomp);
        if(faculty == temp_faculty)
        {
            code = company_room_got_orders(tempcomp, id, room_has_orders);
            if(code != MTM_SUCCESS)
            {
                return code;
            }
            if(room_has_orders)
            {
                return MTM_RESERVATION_EXISTS;
            }
            code = company_remove_room(tempcomp, id);
            return code;
        }
    }
    return MTM_ID_DOES_NOT_EXIST;
}

MtmErrorCode escapetechnion_add_customer(EscapeTechnion escape, char* email,
                                         TechnionFaculty faculty,
                                         int skill_level)
{
    MtmErrorCode code;
    Customer cust = create_customer();
    if(cust == NULL)
    {
        return MTM_OUT_OF_MEMORY;
    }
    code = initialize_customer(cust, email, faculty, skill_level);
    if(code != MTM_SUCCESS)
    {
        customer_destroy(cust);
        return code;
    }
    if(setIsIn(escape->CustomerEmailsSet, email))
    {
        customer_destroy(cust);
        return MTM_EMAIL_ALREADY_EXISTS;
    }
    setAdd(escape->CustomersSet, cust);
    setAdd(escape->CustomerEmailsSet, email);
    return MTM_SUCCESS;
}

MtmErrorCode escapetechnion_remove_customer(EscapeTechnion escape, char* email)
{
    if(escape == NULL || !check_email(email))
    {
        return MTM_INVALID_PARAMETER;
    }
    if(!setIsIn(escape->CustomerEmailsSet, email))
    {
        return MTM_CLIENT_EMAIL_DOES_NOT_EXIST;
    }
    setRemove(escape->CustomersSet, email);
    setRemove(escape->CustomerEmailsSet, email);
    return MTM_SUCCESS;
}

MtmErrorCode escapetechnion_create_order(EscapeTechnion escape, char* email,
                                         TechnionFaculty faculty, int id,
                                         int time, int num_ppl)
{
    MtmErrorCode code;
    Order ord = create_order();
    code = initialize_order(ord, email, faculty, id, time, num_ppl);
    if(code != MTM_SUCCESS)
    {
        return code;
    }
    if(!setIsIn(escape->CustomerEmailsSet, email))
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
    Customer cust;
    code = find_customer_in_set(escape->CustomersSet, email, cust);
    if(code != MTM_SUCCESS)
    {
        order_remove(ord);
        return code;
    }
    if(customer_already_booked(&ord, cust))
    {
        order_remove(ord);
        return MTM_CLIENT_IN_ROOM;
    }
    if(escape_room_colliding_times(room, ord))
    {
        order_remove(ord);
        return MTM_ROOM_NOT_AVAILABLE;
    }
    code = escape_room_add_order(ord);
    if(code != MTM_SUCCESS)
    {
        order_remove(ord);
        return code;
    }
    code = customer_add_order(&ord, cust);
    if(code != MTM_SUCCESS)
    {
        order_remove(ord);
        return code;
    }
    escape->orders_num++;
    return MTM_SUCCESS;
}

MtmErrorCode escapetechnion_recommended_room(EscapeTechnion escape, char* email,
                                             unsigned int num_ppl)
{
    if(escape == NULL || !check_email(email) || num_ppl == 0)
    {
        return MTM_INVALID_PARAMETER;
    }
    if(!setIsIn(escape->CustomerEmailsSet, email))
    {
        return MTM_CLIENT_EMAIL_DOES_NOT_EXIST;
    }
    if(setGetSize(escape->CompanySet) == 0)
    {
        return MTM_NO_ROOMS_AVAILABLE;
    }
    Customer cust;
    MtmErrorCode code;
    code = find_customer_in_set(escape->CustomersSet, email, cust);
    if(code != MTM_SUCCESS)
    {
        return code;
    }
    Company comp;
    comp = setGetFirst(escape->CompanySet);
    bool rooms_exist = false;
    int cur_min = MAX_RECOMMENDED, temp_min, skill = customer_get_skill(cust),
            min_room_id = MAX_RECOMMENDED, temp_room_id, time;
    TechnionFaculty cur_faculty = UNKNOWN, temp_faculty,
            cust_faculty = customer_get_faculty(cust);
    for(int i = 0; i < setGetSize(escape->CompanySet); i++)
    {
        temp_min = company_recommended_rooms(comp, num_ppl, skill,
                                             &temp_room_id, &time);
        temp_faculty = company_get_faculty(comp);
        if(temp_min < MAX_RECOMMENDED && time < MAX_RECOMMENDED)
        {
            rooms_exist = true;
            if(temp_min < cur_min)
            {
                cur_min = temp_min;
                cur_faculty = temp_faculty;
                min_room_id = temp_room_id;
            }
            else if(temp_min == cur_min)
            {
                if(fabs(temp_faculty - cust_faculty) <
                        fabs(cur_faculty - cur_faculty))
                {
                    cur_min = temp_min;
                    cur_faculty = temp_faculty;
                    min_room_id = temp_room_id;
                }
                else if(fabs(temp_faculty - cust_faculty) ==
                        fabs(cur_faculty - cur_faculty))
                {
                    if(temp_faculty < cur_faculty)
                    {
                        cur_min = temp_min;
                        cur_faculty = temp_faculty;
                        min_room_id = temp_room_id;
                    }
                    else if(temp_faculty == cur_faculty)
                    {
                        if(temp_room_id < min_room_id)
                        {
                            cur_min = temp_min;
                            cur_faculty = temp_faculty;
                            min_room_id = temp_room_id;
                        }
                    }
                }
            }
        }
        comp = setGetNext(escape->CompanySet);
    }
    if(!rooms_exist)
    {
        return MTM_NO_ROOMS_AVAILABLE;
    }
    Order ord = create_order();
    code = initialize_order(ord, email, cur_faculty, min_room_id, time,
                            num_ppl);
    if(code != MTM_SUCCESS)
    {
        return code;
    }
    escape->orders_num++;
    return MTM_SUCCESS;
}

MtmErrorCode escapetechnion_reportday(EscapeTechnion escape)
{
    Customer cust;
    cust = setGetFirst(escape->CustomersSet);
    TechnionFaculty faculty;
    Order* sortedord = malloc(sizeof(Order) * (escape->orders_num));
    if(sortedord == NULL)
    {
        return NULL;
    }
    int orders_num, counter = 0, price, sum = 0;
    Order ord;
    for(int i = 0; i < setGetSize(escape->CustomersSet); i++)
    {
        orders_num = customer_get_orders_num(cust);
        if(orders_num > 0)
        {
            for(int j = 0; j < orders_num; j++)
            {
                ord = customer_get_order(cust, j);
                if(order_get_time(ord) < 100)
                {
                    sortedord[counter] = ord;
                    counter++;
                    faculty = order_get_faculty(ord);
                    price = calculate_price(ord, escape);
                    if(faculty == customer_get_faculty(cust))
                    {
                        escape->faculties[faculty] += price * 0.75;
                        sum += price * 0.75;
                    }
                    else
                    {
                        escape->faculties[faculty] += price;
                        sum += price;
                    }
                    customer_remove_order(ord, cust);
                }
                else
                {
                    order_day_passed(ord);
                }
            }
        }
        cust = setGetNext(escape->CustomersSet);
    }
    order_bubble_sort(sortedord, escape);
    escape->days++;
    print_day(escape, counter, sum);
}

MtmErrorCode escapetechnion_reportbest(EscapeTechnion escape)
{
    int no1 = MAX_RECOMMENDED, no2 = MAX_RECOMMENDED, no3 = MAX_RECOMMENDED;
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
