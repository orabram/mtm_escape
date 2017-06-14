#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "set.h"
#include "company.h"

struct company {
    char* email;    //The email of the company
    TechnionFaculty faculty;    //The faculty of the company.
    Set escape_room_set;    //The set which contains all the escape rooms in
                            //the company.
};

/**
 * Receives a SetElement and copies it.
 *
 * @param room: the SetElement we wish to clone.
 * @return
 * Returns Null if room is NULL or if a memory allocation problem occurred.
 * Otherwise, the result of escape_room_copy(room)
 */
static SetElement copy_room(SetElement room)
{
    EscapeRoom copy = create_escape_room();
    if (copy == NULL) {
        return NULL;
    }
    if (escape_room_copy(copy, room) != MTM_SUCCESS) {
        escape_room_destroy(copy);
        return NULL;
    }
    return copy;
}

/**
 * Receives a SetElement and deallocates it.
 *
 * @param room: The SetElement we wish to free.
 */
static void destroy_room(SetElement room)
{
    escape_room_destroy(room);
}

/**
 * Receives two SetElements and compares their ids.
 *
 * @param room1: The first SetElement we wish to compare.
 * @param room2: The second SetElement we wish to compare.
 * @return
 * If the id of room1 is bigger than room2's id, it returns a positive value.
 * If the id of room1 is equal to room2's id, it returns 0.
 * If the id of room1 is smaller than room2's id, it returns a negative value.
 */
static int compare_room(SetElement room1, SetElement room2)
{
    EscapeRoom r1 = (EscapeRoom)room1, r2 = (EscapeRoom)room2;
    return escape_room_get_id(r1) - escape_room_get_id(r2);
}

/**
 * Checks if the e-mail is valid.
 * A valid e-mail is every string containing the '@' character.
 *
 * @param email: the e-mail which is checked
 * @return
 * True if the e-mail is valid,
 * False if it's invalid.
 */
static bool check_email(char* email)
{
    if(!strstr(email, "@"))
    {
        return false;
    }
    return true;
}

//Creates a company and returns it; Initializes it with default values.
Company create_company()
{
    Company company = malloc(sizeof(*company));
    if (company == NULL) {
        return NULL;
    }
    Set room_set = setCreate(copy_room, destroy_room,
                             compare_room);
    if (room_set == NULL) {
        free(company);
        return NULL;
    }
    company->escape_room_set = room_set;
    company->email = NULL;
    return company;
}

//Initializes the company according to the given parameters.
MtmErrorCode initialize_company(Company comp, char* email,
                                TechnionFaculty faculty)
{
    if (comp == NULL) {
        return MTM_NULL_PARAMETER;
    }
    if (!check_email(email) || faculty < 0 || faculty >= UNKNOWN) {
        return MTM_INVALID_PARAMETER;
    }
    comp->email = malloc(strlen(email)+1);
    if (comp->email == NULL) {
        return MTM_OUT_OF_MEMORY;
    }
    strcpy(comp->email, email);
    comp->faculty = faculty;
    return MTM_SUCCESS;
}

//Creates a copy of the company and returns the copy.
Company company_copy(Company comp)
{
    if (comp == NULL) {
        return NULL;
    }
    Company copy = create_company();
    if (copy == NULL) {
        return NULL;
    }
    if (initialize_company(copy, comp->email, comp->faculty)
        == MTM_OUT_OF_MEMORY) {
        company_destroy(copy);
        return NULL;
    }
    return copy;
}

//Adds the given room to the given company.
MtmErrorCode company_add_room(Company comp, EscapeRoom escape)
{
    if (comp == NULL || escape == NULL) {
        return MTM_NULL_PARAMETER;
    }
    if (strcmp(escape_room_get_email(escape), comp->email) != 0) {
        return MTM_INVALID_PARAMETER;
    }
    SetResult add_result = setAdd(comp->escape_room_set, escape);
    switch (add_result) {
        case SET_OUT_OF_MEMORY:
            return MTM_OUT_OF_MEMORY;
        case SET_ITEM_ALREADY_EXISTS:
            return MTM_ID_ALREADY_EXIST;
        default:
            return MTM_SUCCESS;
    }
}

//Removes the room, which is determined by its ID, from the company.
MtmErrorCode company_remove_room(Company comp, int id)
{
    if (comp == NULL) {
        return MTM_NULL_PARAMETER;
    }
    EscapeRoom room = setGetFirst(comp->escape_room_set);
    for (int i = 0; i < setGetSize(comp->escape_room_set); i++) {
        if (escape_room_get_id(room) == id) {
            if (escape_room_order_exists(room)) {
                return MTM_RESERVATION_EXISTS;
            }
            setRemove(comp->escape_room_set, room);
            return MTM_SUCCESS;
        }
    }
    return MTM_ID_DOES_NOT_EXIST;
}

//Returns the company's e-mail.
char* company_get_email(Company comp)
{
    if (comp == NULL) {
        return NULL;
    }
    return comp->email;
}

//Returns the faculty which the company belongs to.
TechnionFaculty company_get_faculty(Company comp)
{
    if (comp == NULL) {
        return UNKNOWN;
    }
    return comp->faculty;
}

int company_get_room_num(Company comp)
{
    if (comp == NULL) {
        return -1;
    }
    return setGetSize(comp->escape_room_set);
}

/* Finds a room with the given ID in the company.
 * NOTE: The ID is unique in the company, so there can be at most one room with
 * such ID.
 */
EscapeRoom company_get_room(Company comp, int id)
{
    if (comp == NULL || id < 0) {
        return NULL;
    }
    EscapeRoom temp_room = setGetFirst(comp->escape_room_set);
    for (int i = 0; i < company_get_room_num(comp); i++) {
        if (escape_room_get_id(temp_room) == id) {
            return temp_room;
        }
        temp_room = setGetNext(comp->escape_room_set);
    }
    return NULL;
}

//Returns True if a room with the given ID exists in the company.
bool company_room_exists(Company comp, int id)
{
    if (company_get_room(comp, id) == NULL) {
        return false;
    }
    return true;
}

/* Calculates the most recommended room for a customer with the given values.
 * The function returns the calculation product as a return value, and the ID of
 * the most recommended room and the soonest day and hour into the given
 * parameters. If one of the passed parameters is invalid, it returns -1.
 */
int company_recommended_rooms(Company comp, int num_ppl, int skill,
                              int* id, int* day, int* hour)
{
    if(comp == NULL || num_ppl < 0 || skill <= 0 || skill > 10 ||
              id == NULL || day == NULL || hour == NULL) {
        return -1;
    }
    int temp_res, result = -1;
    EscapeRoom room = setGetFirst(comp->escape_room_set);
    EscapeRoom best_room = room;
    for (int i = 0; i < setGetSize(comp->escape_room_set); i++) {
        temp_res=escape_room_calculate_recommended_value(room, skill, num_ppl);
        if (temp_res < result || result == -1) {
            result = temp_res;
            *id = escape_room_get_id(room);
            best_room = room;
        }
        room = setGetNext(comp->escape_room_set);
    }
    if (result != -1) {
        if (escape_room_find_closest_time(best_room, day, hour)!=MTM_SUCCESS) {
            return -1;
        }
    }
    return result;
}

//Returns True if at least one order exists in the company, and False otherwise.
bool company_got_orders(Company comp)
{
    if (comp == NULL) {
        return false;
    }
    EscapeRoom room = setGetFirst(comp->escape_room_set);
    for (int i = 0; i < setGetSize(comp->escape_room_set); i++) {
        if (escape_room_order_exists(room)) {
            return true;
        }
        room = setGetNext(comp->escape_room_set);
    }
    return false;
}

MtmErrorCode company_day_passed(Company comp)
{
    if (comp == NULL) {
        return MTM_NULL_PARAMETER;
    }
    int room_num = setGetSize(comp->escape_room_set);
    EscapeRoom room = setGetFirst(comp->escape_room_set);
    for (int i = 0; i < room_num; i++) {
        escape_room_day_passed(room);
        room = setGetNext(comp->escape_room_set);
    }
    return MTM_SUCCESS;
}

//Destroys the company.
void company_destroy(Company comp)
{
    if (comp == NULL) {
        return;
    }
    setDestroy(comp->escape_room_set);
    free(comp->email);
    free(comp);
}
