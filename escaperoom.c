#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "escaperoom.h"

struct escaperoom {
    char* email;
    int id;
    int price;
    int num_ppl;
    int open_hour;
    int close_hour;
    int difficulty;
    Set OrdersSet;
};

static SetElement copy_order(SetElement order)
{
    Order ord = order_copy(order);
    return ord;
}

static void destroy_order(SetElement order)
{
    order_remove(order);
}

static int compare_order(SetElement ord1, SetElement ord2)
{
    if (order_compare_time(ord1, ord2)) {
        return 1;
    }
    if (orders_equal_time(ord1, ord2)) {
        return 0;
    }
    return -1;
}

/** An auxiliary function for setting the working hours of the room
 * It receives working hours in a string format and sets the opening hour and
 * closing hour fields accordingly
 */
static MtmErrorCode set_working_hrs(EscapeRoom room, char *hrs)
{
    if (hrs == NULL)
        return MTM_INVALID_PARAMETER;
    int open_hrs, close_hrs;
    sscanf(hrs, "%d-%d", &open_hrs, &close_hrs);
    if (open_hrs < 0 || open_hrs > 24 || close_hrs < 0 || close_hrs > 24 ||
        open_hrs >= close_hrs) {
        return MTM_INVALID_PARAMETER;
    }
    room->open_hour = open_hrs;
    room->close_hour = close_hrs;
    return MTM_SUCCESS;
}

EscapeRoom create_escape_room()
{
    EscapeRoom room = malloc(sizeof(*room));
    if (room == NULL) {
        return NULL;
    }
    room->OrdersSet = setCreate(copy_order, destroy_order, compare_order);
    if (room->OrdersSet == NULL) {
        escape_room_destroy(room);
        return NULL;
    }
    room->email = NULL;
    room->id = room->price = room->num_ppl = room->open_hour = room->close_hour
    = room->difficulty = 0;
    return room;
}



MtmErrorCode initialize_escape_room(EscapeRoom room, char *email, int id,
                                    int price, int num_ppl, char *working_hrs,
                                    int difficulty)
{
    if (room == NULL )
    if ( id < 0 || price % 4 != 0 || price <= 0 || num_ppl <= 0
         || difficulty < 1 || difficulty > 10) {
        return MTM_INVALID_PARAMETER;
    }
    if (set_working_hrs(room, working_hrs) == MTM_INVALID_PARAMETER) {
        return MTM_INVALID_PARAMETER;
    }
    room->id = id;
    room->num_ppl = num_ppl;
    room->difficulty = difficulty;
    room->email = malloc(strlen(email) + 1);
    if (room->email == NULL) {
        return MTM_OUT_OF_MEMORY;
    }
    strcpy(room->email, email);
    return MTM_SUCCESS;
}

char* escape_room_get_email(EscapeRoom room)
{
    assert(room != NULL);
    assert(room->email != NULL);
    return room->email;
}

int escape_room_get_id(EscapeRoom room)
{
    assert(room != NULL);
    assert(room->id >= 0);
    return room->id;
}

int get_room_price(EscapeRoom room)
{
    assert(room != NULL);
    assert(room->price > 0);
    assert(room->price % 4 == 0);
    return room->price;
}

int get_room_num_ppl(EscapeRoom room)
{
    assert(room != NULL);
    assert(room->num_ppl > 0);
    return room->num_ppl;
}

char* get_room_working_hrs(EscapeRoom room)
{
    assert(room != NULL);
    assert(room->open_hour >= 0 && room->open_hour < 24);
    assert(room->close_hour >= 0 && room->close_hour <= 24);
    assert(room->close_hour > room->open_hour);
    char* working_hrs = malloc(strlen("00-00")+1);
    sprintf(working_hrs, "%d-%d", room->open_hour, room->close_hour);
    return working_hrs;
}

int get_room_difficulty(EscapeRoom room)
{
    assert(room != NULL);
    assert(room->difficulty >=1 && room->difficulty <= 10);
    return room->difficulty;
}

int escape_room_calculate_recommended_value(EscapeRoom room, int skill_level,
                                            int num_ppl)
{
    if (room == NULL) {
        return -1;
    }
    return ((room->num_ppl - num_ppl)*(room->num_ppl - num_ppl)) +
            ((room->difficulty - skill_level)*(room->difficulty - skill_level));
}

MtmErrorCode escape_room_add_order(EscapeRoom room, Order order)
{
    if (room == NULL || order == NULL) {
        return MTM_NULL_PARAMETER;
    }
    int order_time = order_get_hour(order);
    if (order_time < room->open_hour || order_time >= room->close_hour) {
        return MTM_INVALID_PARAMETER;
    }
    if (setAdd(room->OrdersSet, order) == SET_OUT_OF_MEMORY) {
        return MTM_OUT_OF_MEMORY;
    }
    return MTM_SUCCESS;
}

bool escape_room_colliding_times(EscapeRoom room, Order order)
{
    if (room == NULL || order == NULL) {
        return false;
    }
    int order_time = order_get_hour(order);
    Order ord = setGetFirst(room->OrdersSet);
    for (int i = 0; i < setGetSize(room->OrdersSet); i++) {
        if (order_get_hour(ord) == order_time) {
            return true;
        }
        ord = setGetNext(room->OrdersSet);
    }
    return false;
}

MtmErrorCode escape_room_remove_order(EscapeRoom room, Order order)
{
    if (room == NULL || order == NULL) {
        return MTM_NULL_PARAMETER;
    }
    setRemove(room->OrdersSet, order);
    return MTM_SUCCESS;
}

bool escape_room_order_exists(EscapeRoom room)
{
    if (room == NULL) {
        return false;
    }
    if (setGetSize(room->OrdersSet) > 0) {
        return true;
    }
    return false;
}


void escape_room_destroy(EscapeRoom room)
{
    if (room == NULL) {
        return;
    }
    free(room->email);
    setDestroy(room->OrdersSet);
    room->email = NULL;
    free(room);
}
