#ifndef COMPANY_H_
#define COMPANY_H_

#include <stdio.h>
#include <stdbool.h>
#include "set.h"
#include "escaperoom.h"
#include "mtm_ex3.h"

typedef struct company *Company;

Company create_company();

//Initializes a new company.
MtmErrorCode initialize_company(Company comp, char* email,
                                TechnionFaculty faculty);

Company company_copy(Company comp);

//Adds a room to the company.
MtmErrorCode company_add_room(Company comp, EscapeRoom escape);

/**
 * Removes the room with the given ID from the company
 *
 * @param comp - The company on which the function operates
 * @param id - ID of the room that should be removed
 * @return
 * MTM_INVALID_PARAMETER if a invalid parameter was sent
 * MTM_RESERVATION_EXISTS if the room has an existing reservation and therefore
 *                        cannot be removed
 * MTM_ID_DOES_NOT_EXIST if the room does not exist in the company
 * MTM_SUCCESS if the function succeeded removing the room
 */
MtmErrorCode company_remove_room(Company comp, int id);

//Changes the email of the company. It also affects all the room it stores.
MtmErrorCode company_set_email(Company comp, char* email);

//Returns the company email into the given variable.
char* company_get_email(Company comp);

TechnionFaculty company_get_faculty(Company comp);

int company_get_room_num(Company comp);

EscapeRoom company_get_room(Company comp, int id);

bool company_room_exists(Company comp, int id);

int company_recommended_rooms(Company comp, unsigned int num_ppl, int skill,
                              int id, int day, int hour);

bool company_room_got_orders(Company comp, int id);

bool company_got_orders(Company comp);

void company_destroy(Company comp);

#endif