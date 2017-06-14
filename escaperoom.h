#ifndef ESCAPEROOM_H_
#define ESCAPEROOM_H_

#include <stdio.h>
#include <stdbool.h>
#include "set.h"
#include "order.h"

typedef struct escaperoom *EscapeRoom;

/**
 * Creates a new instance of EscapeRoom.
 * Allocates all necessary space for it and initializes with default values.
 *
 * @return
 * NULL if a memory allocation problem occurred.
 * The newly created instance of EscapeRoom otherwise.
 */
EscapeRoom create_escape_room();


/**
 * Initializes the given EscapeRoom with values passed as parameters.
 *
 * @param room: The room to initialize.
 * @param email: The e-mail of the room to initialize.
 * @param id: The ID of the room to initialize.
 * @param price: The price for an order per person in the room.
 * @param num_ppl: The recommended number of people to visit the room at once.
 * @param working_hrs: The working hours of the room, passed as a string in
 *                     "xx-yy" format, where "xx" is the opening hour and
 *                     "yy" is the closing hour.
 * @param difficulty: The difficulty level of the room.
 * @return
 * MTM_NULL_PARAMETER if the room is NULL
 * MTM_INVALID_PARAMETER if at least one of the parameters is invalid
 * MTM_OUT_OF_MEMORY if a memory allocation problem occurred
 * MTM_SUCCESS otherwise
 */
MtmErrorCode initialize_escape_room(EscapeRoom room, char* email, int id,
                                    int price, int num_ppl, char *working_hrs,
                                    int difficulty);

/**
 * Creates a copy of the room and returns it in the dedicated parameter.
 *
 * @param new_room: The copy of the room will be returned here.
 * @param original_room: The original room, of which a copy is created.
 * @return
 * MTM_NULL_PARAMETER if a NULL parameter was sent. new_room is NULL.
 * MTM_OUT_OF_MEMORY if a memory allocation problem occurred. new_room is NULL.
 * MTM_SUCCESS otherwise
 */
MtmErrorCode escape_room_copy(EscapeRoom new_room, EscapeRoom original_room);

/**
 * Returns the e-mail of the room.
 *
 * @param room: The room on which the function operates.
 * @return
 * NULL if the room that was sent is NULL
 * The e-mail of the room
 */
char* escape_room_get_email(EscapeRoom room);

/**
 * Returns the ID of the room.
 *
 * @param room: The room on which the function operates.
 * @return
 * -1 if the room that was sent is NULL
 * The ID of the room
 */
int escape_room_get_id(EscapeRoom room);

/**
 * Returns the price of an order per person in the room.
 *
 * @param room: The room on which the function operates.
 * @return
 * -1 if the room that was sent is NULL
 * The price as described above
 */
int escape_room_get_price(EscapeRoom room);

/**
 * Returns the working hours of the room as a string in format "xx-yy",
 * where "xx" is the opening hour and "yy" is the closing hour of the room.
 *
 * @param room: The room on which the function operates.
 * @return
 * NULL if the room that was sent is NULL
 * The working hours of the room
 */
char* get_room_working_hrs(EscapeRoom room);

/**
 * Returns the difficulty level of the room.
 *
 * @param room: The room on which the function operates.
 * @return
 * -1 if the room that was sent is NULL
 * The difficulty level of the room.
 */
int escape_room_get_difficulty(EscapeRoom room);

/**
 * Calculates the recommendation value of the room according to the formula:
 * (P_r - P_e)^2 + (difficulty - skill level)^2, where:
 *  P_r is the recommended number of people for the room;
 *  P_e is the number of people in the order;
 *  difficulty is the difficulty level of the room;
 *  skill level is the skill level of the customer.
 *
 * @param room: The room for which the calculation is done.
 * @param skill_level: The skill level of the customer.
 * @param num_ppl: The number of people in the order.
 * @return
 * -1 if the room that was sent is NULL
 * The calculation product
 */
int escape_room_calculate_recommended_value(EscapeRoom room, int skill_level,
                                            int num_ppl);

/**
 * Finds the soonest time when the room will be available and returns it into
 * the given parameters for day and hour
 *
 * @param room: The room on which the function operates
 * @param day: The soonest day available will be returned here
 * @param hour: The soonest hour available will be returned here
 * @return
 * MTM_NULL_PARAMETER if the room is NULL
 * MTM_SUCCESS otherwise
 */
MtmErrorCode escape_room_find_closest_time(EscapeRoom room,int *day,int *hour);

/**
 * Adds a copy of the requested order into the room.
 *
 * @param room: The room which the order is added to.
 * @param order: The requested order to be added into the room
 * @return
 * MTM_NULL_PARAMETER if the room or the order is NULL
 * MTM_INVALID_PARAMETER if the time of the order is beyond the working hours
 *                       of the room
 * MTM_RESERVATION_EXISTS if a reservation for the same time already exists
 *                        in this room.
 * MTM_OUT_OF_MEMORY if the allocation of the copy of the order failed
 * MTM_SUCCESS otherwise
 */
MtmErrorCode escape_room_add_order(EscapeRoom room, Order order);

/**
 * Removes the requested order from the room.
 *
 * @param room: The room which the order is removed from.
 * @param order: The requested order to be removed.
 * @return
 * MTM_NULL_PARAMETER if if the room or the order is NULL
 * MTM_ROOM_NOT_AVAILABLE if such order does not exist in this room
 * MTM_SUCCESS otherwise
 */
MtmErrorCode escape_room_remove_order(EscapeRoom room, Order order);

/**
 * Checks if the room has any orders currently booked.
 *
 * @param room: The checked room
 * @return
 * True if the room has at least one order
 * False otherwise
 */
bool escape_room_order_exists(EscapeRoom room);

/**
 * Deallocates the room and all its fields.
 *
 * @param room: The room to destroy
 */
void escape_room_destroy(EscapeRoom room);

#endif