#ifndef COMPANY_H_
#define COMPANY_H_

#include <stdio.h>
#include <stdbool.h>
#include "set.h"
#include "escaperoom.h

typedef struct company{
    char** email;
    TechnionFaculty faculty;
    Set escape_room_set;
} *Company;

//Initializes a new company.
MtmErrorCode create_company(Company comp, char* email, TechnionFaculty faculty);

MtmErrorCode company_copy(Company comp);

//Adds a room to the company.
MtmErrorCode company_add_room(Company comp, EscapeRoom escape);

//Removes a room from the company.
MtmErrorCode company_remove_room(Company comp, EscapeRoom);

//Changes the email of the company. It also affects all the room it stores.
MtmErrorCode company_set_email(Company comp, char* email);

//Returns the company email into the given variable.
MtmErrorCode company_get_email(Company comp, char* email);

MtmErrorCode company_destroy(Company comp);

#endif