#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "set.h"
#include "company.h"

struct company {
    char* email;
    TechnionFaculty faculty;
    Set escape_room_set;
};

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

static void destroy_room(SetElement room)
{
    escape_room_destroy(room);
}

static int compare_room(SetElement room1, SetElement room2)
{
    EscapeRoom r1 = (EscapeRoom)room1, r2 = (EscapeRoom)room2;
    return escape_room_get_id(r1) - escape_room_get_id(r2);
}

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
    return company;
}

MtmErrorCode initialize_company(Company comp, char* email,
                                TechnionFaculty faculty)
{
    if (comp == NULL) {
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

MtmErrorCode company_add_room(Company comp, EscapeRoom escape)
{
    if (comp == NULL || escape == NULL) {
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

MtmErrorCode company_remove_room(Company comp, int id)
{
    if (comp == NULL) {
        return MTM_INVALID_PARAMETER;
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

char* company_get_email(Company comp)
{
    if (comp == NULL) {
        return NULL;
    }
    return comp->email;
}

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

bool company_room_exists(Company comp, int id)
{
    if (company_get_room(comp, id) == NULL) {
        return false;
    }
    return true;
}

int company_recommended_rooms(Company comp, int num_ppl, int skill,
                              int* id, int* day, int* hour)
{
    assert (!(comp == NULL || num_ppl < 0 || skill <= 0 || skill > 10 ||
            id == NULL || day == NULL || hour == NULL));
    int temp_res, result = -1;
    EscapeRoom room = setGetFirst(comp->escape_room_set);
    for (int i = 0; i < setGetSize(comp->escape_room_set); i++) {
        temp_res=escape_room_calculate_recommended_value(room, skill, num_ppl);
        if (temp_res < result || result == -1) {
            result = temp_res;
        }
        room = setGetNext(comp->escape_room_set);
    }
    return result;
}

/*bool company_room_got_orders(Company comp, int id)
{
    if (comp == NULL || id < 0) {
        return false;
    }
    EscapeRoom room = company_get_room(comp, id);
    if (room == NULL) {
        return false;
    }
    if (escape_room_order_exists(room)) {
        return true;
    }
    return false;
}*/

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

void company_destroy(Company comp)
{
    if (comp == NULL) {
        return;
    }
    setDestroy(comp->escape_room_set);
    free(comp->email);
    free(comp);
}


