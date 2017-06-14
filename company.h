#ifndef COMPANY_H_
#define COMPANY_H_

#include <stdio.h>
#include <stdbool.h>
#include "set.h"
#include "escaperoom.h"
#include "mtm_ex3.h"

typedef struct company *Company;

/**
 * Allocates space for a new Company object.
 *
 * @return
 * NULL upon memory failure in either the Company creation or the room_set
 * creation.
 * A Company object otherwise.
 */
Company create_company();

/**
 * Receives multiple parameters and initializes with the
 * a newly created Company.
 *
 * @param comp: The Company object this function initializes.
 * @param email: The email of the company.
 * @param faculty: The faculty of the company
 * @return
 * MTM_OUT_OF_MEMORY if there was an allocation problem with email.
 * MTM_NULL_PARAMETER if either comp or email is NULL.
 * MTM_INVALID_PARAMETER if email doesn't contain at least one @ or if
 * faculty's numeral value is either lower than 0 or greater than UNKNOWN'S.
 *
 */
MtmErrorCode initialize_company(Company comp, char* email,
                                TechnionFaculty faculty);
/**
 * Receives an existing company and copies it.
 *
 * @param comp: The company we wish to copy.
 * @return
 * NULL if comp is NULL or if the memory allocation failed.
 * A cloned Company otherwise.
 */
Company company_copy(Company comp);

/**
 * Adds a new EscapeRoom to company.
 *
 * @param comp: The company to which we'll add the room.
 * @param escape: The room we'll add.
 * @return
 * MTM_OUT_OF_MEMORY: if the set has memory allocation problems.
 * MTM_NULL_PARAMETER: if either comp or escape are equal to NULL.
 * MTM_INVALID_PARAMETER: if their email addresses aren't equal(room should
 * have the email address of 
 */
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

//Returns the company email into the given variable.
char* company_get_email(Company comp);

TechnionFaculty company_get_faculty(Company comp);

int company_get_room_num(Company comp);

EscapeRoom company_get_room(Company comp, int id);

bool company_room_exists(Company comp, int id);

int company_recommended_rooms(Company comp, int num_ppl, int skill,
                              int* id, int* day, int* hour);

//bool company_room_got_orders(Company comp, int id);

bool company_got_orders(Company comp);

void company_destroy(Company comp);

#endif