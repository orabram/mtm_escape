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

/**
 * Receives a company and returns its email address.
 *
 * @param comp: The company containing the email we wish to return.
 * @return
 * NULL if either comp or comp's email are equal to NULL.
 * An email address otherwise.
 */
char* company_get_email(Company comp);

/**
 * Receives a company and returns its faculty.
 *
 * @param comp: The company containing the faculty we wish to return.
 * @return
 * UNKNOWN if comp is equal to NULL.
 * Company's faculty otherwise.
 */
TechnionFaculty company_get_faculty(Company comp);

/**
 * Receives a company and returns the number of rooms it has.
 *
 * @param comp: The company containing the rooms.
 * @return
 * -1 if comp is equal to NULL.
 * Company's room num otherwise.
 */
int company_get_room_num(Company comp);

/**
 * Receives a company and returns a room with a given id.
 *
 * @param comp: The company containing the room we wish to return.
 * @param id: The id of the room in the faculty. Must be a positive integer.
 * @return
 * NULL if comp is equal to NULL, id is lower than 0 or if the room doesn't
 * exist.
 * The room will be returned otherwise.
 */
EscapeRoom company_get_room(Company comp, int id);

/**
 * Receives a company and room id and check if the room exists in this company.
 * @param comp: The company contating the rooms.
 * @param id: The id of the room in the faculty. Must be a positive integer.
 * @return
 * false if comp is NULL, id is lower than 0 or if the room doesn't exist.
 * true otherwise.
 */
bool company_room_exists(Company comp, int id);

/**
 * Receives a company, the amount of people making a reservation, the skill of
 * the maker, and three pointers. Returns the most recommended room in the
 * company, and stores the day and  the hour in which the room will be free
 * next and the id of the given room.
 * @param comp: The company containing the rooms.
 * @param num_ppl: The number of people in the reservation. Must be a positive
 * integer.
 * @param skill: The skill level of the customer. Must be an integer between
 * 1 to 10.
 * @param id: int* in which we store the id of the chosen room.
 * @param day: int* in which we store the days until the reservation.
 * @param hour: int* in which we store the hours until the reservation.
 * @return
 * Returns -1 if comp, id, day or hour are NULL, if any paramater doesn't
 * match their criteria, if there aren't any rooms or if the
 * function find_closest_time fails.
 * Returns the result of the given equation on success.
 */
int company_recommended_rooms(Company comp, int num_ppl, int skill,
                              int* id, int* day, int* hour);

//bool company_room_got_orders(Company comp, int id);
/**
 * Receives company and returns true or false, depends on whether it has
 * pending orders.
 * @param comp: The company containing the orders.
 * @return
 * false if comp is NULL or if there aren't any orders.
 * true otherwise.
 */
bool company_got_orders(Company comp);

/**
 * Updates the rooms of the company after a day passed
 *
 * @param comp: The company to update
 * @return
 * MTM_NULL_PARAMETER if the sent company is NULL
 * MTM_SUCCESS otherwise.
 */
MtmErrorCode company_day_passed(Company comp);
/**
 * Destroy comp and frees all the memory is has allocated.
 * @param comp: The company that'll be destroyed.
 */
void company_destroy(Company comp);

#endif