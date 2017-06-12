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

Company create_company()
{
    Company company = malloc(sizeof(*company));
    if (company == NULL) {
        return NULL;
    }

    /**
     * Got to change these functions according to the Set requirements
     */
    Set room_set = setCreate(escape_room_copy, escape_room_destroy,
                             escape_room_compare);

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
        return NULL;
    }
    return comp->faculty;
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


