#ifndef ESCAPEROOM_H_
#define ESCAPEROOM_H_

#include <stdio.h>
#include <stdbool.h>
#include "set.h"
#include "order.h"

typedef struct escaperoom *EscapeRoom;

//Creates a new escape room. Resets OrdersSet.
EscapeRoom create_escape_room(char* email, int id, int price,
                                int num_ppl, char *working_hrs, int difficulty);

//Copies an existing escape room.
MtmErrorCode escape_room_copy(EscapeRoom new_room, EscapeRoom original_room);

//Switches the current corporate email with a new one.
//NOTE: It does not change the original one.
MtmErrorCode escape_room_set_emails(EscapeRoom escape, char* email);

//Changes the current room id.
//NOTE: All recorded orders in the room are updated as well.
MtmErrorCode escape_room_set_id(EscapeRoom escape, int id);

//Changes the current price.
MtmErrorCode escape_room_set_price(EscapeRoom escape, int price);

//Changes the current recommended amount of people in the room.
MtmErrorCode escape_room_set_num_ppl(EscapeRoom escape, int num_ppl);

//Changes the working hours of the room.
//NOTE: Due to this function not being vital, we've removed it until futher
//notice.
/*MtmErrorCode escape_room_set_working_hrs(EscapeRoom escape, int working_hrs);*/

//Changes the difficulty of the room.
MtmErrorCode escape_room_set_difficulty(EscapeRoom escape, int difficulty);

//Changes the current price.
MtmErrorCode escape_room_set_faculty(EscapeRoom escape, TechnionFaculty faculty);

//Returns the email into the given variable.
MtmErrorCode escape_room_get_email(EscapeRoom room, char* email);

//Returns the id into the given variable.
MtmErrorCode escape_room_get_id(EscapeRoom room, int* id);

//Returns the price into the given variable.
MtmErrorCode escape_room_get_price(EscapeRoom room, int* price);

//Returns the num_ppl into the given variable.
MtmErrorCode escape_room_get_num_ppl(EscapeRoom room, int* num_ppl);

//Returns the working_hrs into the given variable.
MtmErrorCode escape_room_get_working_hrs(EscapeRoom room, char **working_hrs);

//Returns the difficulty into the given variable.
MtmErrorCode escape_room_get_difficulty(EscapeRoom room, int* difficulty);

//Returns the faculty into the given variable.
MtmErrorCode escape_room_get_faculty(EscapeRoom room,
                                     TechnionFaculty *faculty);

//Calculates how recommended this room is to a specific party,
// and stores the value in the given variable.
MtmErrorCode escape_room_calculate_recommended_value(EscapeRoom room,
                                                     int skill_level,
                                                     int party_size,
                                                     int* recommended_value);

//Adds a new order to the room.
MtmErrorCode escape_room_add_order(EscapeRoom room, Order order);

//Checks to see if two orders match each other's time.
MtmErrorCode escape_room_colliding_times(EscapeRoom room, Order order);

//Removes an order.
MtmErrorCode escape_room_remove_order(EscapeRoom room, Order order);

//Destroys the room.
MtmErrorCode escape_room_destroy(EscapeRoom room, Order order);

#endif