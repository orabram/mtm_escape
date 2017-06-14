#ifndef ESCAPETECHNION_H_
#define ESCAPETECHNION_H_

#include <stdio.h>
#include <stdbool.h>
#include "company.h"
#include "customer.h"
#include "list.h"

typedef struct escapetechnion *EscapeTechnion;

/**
 * Creates a new instance of EscapeTechnion and returns it
 *
 * @return
 * NULL if memory allocation problem occurred
 * The new instance of EscapeTechnion otherwise
 */
EscapeTechnion create_escapetechnion();

/**
 * Sets an output channel for escapetechnion
 * (In order to print all its messages)
 *
 * @param escape: The escapetechnion object for which the channel is set
 * @param output_channel: The output channel to set
 * @return
 * MTM_NULL_PARAMETER if one of the parameters is NULL
 * MTM_SUCCESS otherwise
 */
MtmErrorCode escapetechnion_set_output_channel(EscapeTechnion escape,
                                               FILE* output_channel);

/**
 * Adds a company with the given e-mail and faculty to escapetechnion
 *
 * @param escape: The escapetechnion object
 * @param email: The e-mail of the new company
 * @param faculty: The faculty which the company belongs to
 * @return
 * MTM_NULL_PARAMETER if escape is NULL
 * MTM_OUT_OF_MEMORY if memory allocation problem occurred
 * MTM_EMAIL_ALREADY_EXISTS if a company with the same e-mail already exists
 * MTM_SUCCESS otherwise
 */
MtmErrorCode escapetechnion_add_company(EscapeTechnion escape,
                                        char* email, TechnionFaculty faculty);

/**
 * Removes the company with the given e-mail from the escapetechnion object
 *
 * @param escape: The escapetechnion object
 * @param email: The e-mail of the company to remove
 * @return
 * MTM_NULL_PARAMETER if escape is NULL
 * MTM_INVALID_PARAMETER if the e-mail is invalid
 * MTM_COMPANY_EMAIL_DOES_NOT_EXIST if no company with such e-mail exists
 * MTM_RESERVATION_EXISTS if the company has reservations and cannot be removed
 * MTM_SUCCESS otherwise
 */
MtmErrorCode escapetechnion_remove_company(EscapeTechnion escape, char* email);

/**
 * Creates a room for a company with the given e-mail with the given parameters
 * and adds it into it.
 *
 * @param escape: The escapetechnion object in which the room will be created
 * @param email: The e-mail of the company of the room
 * @param id: The ID of the room
 * @param price: The price for a visit in the room per person
 * @param num_ppl: The recommended number of people to visit the room at once
 * @param working_hrs: The working hours of the room in a string format
 * @param difficulty: The difficulty level of the room
 * @return
 * MTM_NULL_PARAMETER if escape is NULL
 * MTM_OUT_OF_MEMORY if memory allocation problem occurred
 * MTM_COMPANY_EMAIL_DOES_NOT_EXIST if there's no company with such e-mail
 * MTM_ID_ALREADY_EXIST if a room with such ID already exists
 * MTM_SUCCESS otherwise
 */
MtmErrorCode escapetechnion_add_room(EscapeTechnion escape, char* email,
                                     int id, int price, int num_ppl,
                                     char* working_hrs, int difficulty);

/**
 * Removes a room from its company, determined by company's faculty and room's
 * ID.
 *
 * @param escape: The escapetechnion object
 * @param faculty: The faculty which the room belongs to
 * @param id: The ID of the room
 * @return
 * MTM_NULL_PARAMETER if escape is NULL
 * MTM_INVALID_PARAMETER if the sent faculty is invalid
 * MTM_RESERVATION_EXISTS if there is a reservation for this room, and therefore
 *                        it cannot be removed
 * MTM_ID_DOES_NOT_EXIST if a room with such ID does not exist
 * MTM_SUCCESS otherwise
 */
MtmErrorCode escapetechnion_remove_room(EscapeTechnion escape,
                                        TechnionFaculty faculty, int id);

/**
 * Adds a customer with the given values to escapetechnion
 *
 * @param escape: The escapetechnion object
 * @param email: The customer's e-mail
 * @param faculty: The customer's faculty
 * @param skill_level: The customer's skill level
 * @return
 * MTM_NULL_PARAMETER if escape is NULL
 * MTM_OUT_OF_MEMORY if memory allocation problem occurred
 * MTM_EMAIL_ALREADY_EXISTS if a customer or a company with such e-mail already
 *                          exists
 * MTM_SUCCESS otherwise
 */
MtmErrorCode escapetechnion_add_customer(EscapeTechnion escape, char* email,
                                         TechnionFaculty faculty,
                                         int skill_level);

/**
 * Removes the customer with the given e-mail from the escapetechnion objcect
 *
 * @param escape: The escapetechnion object
 * @param email: The e-mail of the customer to remove
 * @return
 * MTM_NULL_PARAMETER if escape is NULL
 * MTM_INVALID_PARAMETER if the e-mail is invalid
 * MTM_CLIENT_EMAIL_DOES_NOT_EXIST if no client with such email exists
 * MTM_SUCCESS otherwise
 */
MtmErrorCode escapetechnion_remove_customer(EscapeTechnion escape, char* email);

/**
 * Creates order for a customer at a room with the given identifying values,
 * and initializes it with another given values
 *
 * @param escape: The escapetechnion object
 * @param email: The customer's e-mail
 * @param faculty: The faculty of the room for which the order is created
 * @param id: The ID of the room for which the order is created
 * @param time: The time of the order, given in a format of "day-hour" as a
 *              string, where "day" is the number of days from today until
 *              the order, and "hour" is the hour of the order
 * @param num_ppl: The number of people requesting the order
 * @return
 * MTM_NULL_PARAMETER if escape is NULL
 * MTM_OUT_OF_MEMORY if a memory allocation problem occurred
 * MTM_CLIENT_EMAIL_DOES_NOT_EXIST if no client with such email exists
 * MTM_ID_DOES_NOT_EXIST if no room with such ID exists
 * MTM_CLIENT_IN_ROOM if the client already has an order for this time.
 * MTM_ROOM_NOT_AVAILABLE if the room is not available at this time
 * MTM_SUCCESS otherwise
 */
MtmErrorCode escapetechnion_create_order(EscapeTechnion escape, char* email,
                                         TechnionFaculty faculty, int id,
                                         char* time, int num_ppl);

/**
 * Finds the most recommended room for a client with the given e-mail for the
 * given number of people, and books a reservation at it as soon as possible
 *
 * @param escape: The escapetechnion object
 * @param email: The e-mail of the customer
 * @param num_ppl: The number of people for which the recommendation and
 *                 reservation apply
 * @return
 * MTM_NULL_PARAMETER if escape is NULL
 * MTM_INVALID_PARAMETER if the e-mail or the number of people is invalid
 * MTM_NO_ROOMS_AVAILABLE if no rooms available to book
 * MTM_SUCCESS otherwise
 */
MtmErrorCode escapetechnion_recommended_room(EscapeTechnion escape, char* email,
                                             int num_ppl);

/**
 * Prints a report about all orders tha took place last day.
 * The function also advances the day of the system by 1, and removes all the
 * orders of the recent day passed.
 *
 * @param escape: The escapetechnion object on which the function operates
 * @return
 * MTM_NULL_PARAMETER if escape is NULL
 * MTM_OUT_OF_MEMORY if memory allocation problem occurred
 * MTM_SUCCESS otherwise
 */
MtmErrorCode escapetechnion_reportday(EscapeTechnion escape);

void escapetechnion_reportbest(EscapeTechnion escape);

void escapetechnion_destroy(EscapeTechnion escape);

#endif