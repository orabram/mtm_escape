/*#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "escaperoom.h"

struct escaperoom {
    char** email;
    int id;
    int price;
    int num_ppl;
    int open_hour;
    int close_hour;
    int difficulty;
    Set OrdersSet;
};

static MtmErrorCode set_working_hrs(EscapeRoom room, char *hrs);

EscapeRoom create_escape_room(char *email, int id, int price,
                              int num_ppl, char *working_hrs, int difficulty) {
    EscapeRoom room = malloc(sizeof(*room));
    if (room == NULL)
        return NULL;
    if (id < 0 || price % 4 != 0 || price <= 0 || num_ppl <= 0 ||
            difficulty < 1 || difficulty > 10) {
        free(room);
        return NULL;
    }
    room->id = id;
    room->num_ppl = num_ppl;
    room->difficulty = difficulty;
    if (set_working_hrs(room, working_hrs) == MTM_INVALID_PARAMETER) {
        free(room);
        return NULL;
    }
    room->email = malloc(strlen(email) + 1);
    if (room->email == NULL) {
        free(room);
        return NULL;
    }
    strcpy(room->email, email);
    return room;
}

MtmErrorCode reset_escape_room(EscapeRoom room)
{
    free(room->email);
    room->email = NULL;
    room->id = 0;
    room->price = 0;
    room->num_ppl = 0;
    room->open_hour = 0;
    room->close_hour = 0;
    room->difficulty = 0;
    return MTM_SUCCESS;
}

MtmErrorCode escape_room_get_email(EscapeRoom room, char *email)
{
    assert(room->email != NULL);
    strcpy(email, room->email);
    return MTM_SUCCESS;
}

MtmErrorCode escape_room_get_id(EscapeRoom room, int* id)
{
    assert(room->id >= 0);
    *id = room->id;
    return MTM_SUCCESS;
}
MtmErrorCode get_room_price(EscapeRoom room, int *price)
{
    assert(room->price > 0);
    assert(room->price % 4 == 0);
    *price = room->price;
    return MTM_SUCCESS;
}

MtmErrorCode get_room_num_ppl(EscapeRoom room, int *num_ppl)
{
    assert(room->num_ppl > 0);
    *num_ppl = room->num_ppl;
    return MTM_SUCCESS;
}

MtmErrorCode get_room_working_hrs(EscapeRoom room, char **working_hrs)
{
    assert(room->open_hour >= 0 && room->open_hour <= 24);
    assert(room->close_hour >= 0 && room->close_hour <= 24);
    assert(room->close_hour > room->open_hour);
    sprintf(*working_hrs, "%d-%d", room->open_hour, room->close_hour);
    return MTM_SUCCESS;
}

MtmErrorCode get_room_difficulty(EscapeRoom room, int *difficulty)
{
    assert(room->difficulty >=1 && room->difficulty <= 10);
    *difficulty = room->difficulty;
    return MTM_SUCCESS;
}


 * An auxiliary function for setting the working hours of the room
 * It receives working hours in a string format and sets the opening hour and
 * closing hour fields accordingly

static MtmErrorCode set_working_hrs(EscapeRoom room, char *hrs)
{
    if (hrs == NULL)
        return MTM_INVALID_PARAMETER;
    int open_hrs, close_hrs;
    sscanf(hrs, "%d-%d", open_hrs, close_hrs);
    if (open_hrs < 0 || open_hrs > 24 || close_hrs < 0 || close_hrs > 24 ||
        open_hrs >= close_hrs) {
        return MTM_INVALID_PARAMETER;
    }
    room->open_hour = open_hrs;
    room->close_hour = close_hrs;
    return MTM_SUCCESS;
}
*/