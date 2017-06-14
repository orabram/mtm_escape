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

/**
 * Creates a copy of the order and returns it.
 *
 * @param order: The order to copy
 * @return
 * Return value of order_copy function
 */
static SetElement copy_order(SetElement order)
{
    Order ord = order_copy(order);
    return ord;
}

/**
 * Deallocates the order.
 *
 * @param order: The order to destroy
 */
static void destroy_order(SetElement order)
{
    order_remove(order);
}

/**
 * Compares between two orders by their times of booking.
 *
 * @param ord1: First order to compare
 * @param ord2: Second order to compare
 * @return
 * 1 if ord1 is set for later than ord2
 * 0 if both orders are set for the same time
 * -1 if ord2 is set for later than ord1
 */
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

/**
 * Checks if the e-mail is valid.
 * A valid e-mail is any string containing the '@' character.
 *
 * @param email: The e-mail to check
 * @return
 * True if the e-mail is valid
 * False otherwise
 */
static bool check_email(char* email)
{
    if(!strstr(email, "@")) //Checks if email contains at least 1 @.
    {
        return false;
    }
    if(strstr(strstr(email, "@") + 1, "@")) //Checks if email contains more
        // than one @.
    {
        return false;
    }
    return true;
}
}

/** An auxiliary function for setting the working hours of the room.
 *  Receives working hours in a string format and sets the opening hour and
 *  closing hour fields respectively
 *
 * @param room: The room on which the function operates
 * @param hrs: Working hours of the room in a string format
 * @return
 * MTM_INVALID_PARAMETER if hrs is NULL
 * MTM_SUCCESS otherwise
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

/**
 * Checks if a reservation for the same time already exists in this room.
 *
 * @param room: The checked room.
 * @param order: The checked order.
 * @return
 * True if such order exists
 * False otherwise
 */
static bool escape_room_colliding_times(EscapeRoom room, Order order)
{
    if (room == NULL || order == NULL) {
        return false;
    }
    int order_day = order_get_day(order), order_hour = order_get_hour(order);
    Order ord = setGetFirst(room->OrdersSet);
    for (int i = 0; i < setGetSize(room->OrdersSet); i++) {
        if (order_get_day(ord) == order_day &&
                order_get_hour(ord) == order_hour) {
            return true;
        }
        ord = setGetNext(room->OrdersSet);
    }
    return false;
}

//Creates a new EscapeRoom and returns it
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

//Initializes the given room with the given parameters
MtmErrorCode initialize_escape_room(EscapeRoom room, char *email, int id,
                                    int price, int num_ppl, char *working_hrs,
                                    int difficulty)
{
    if (room == NULL ) {
        return MTM_NULL_PARAMETER;
    }
    if ( id < 0 || price % 4 != 0 || price <= 0 || num_ppl <= 0
         || difficulty < 1 || difficulty > 10) {
        return MTM_INVALID_PARAMETER;
    }
    if (set_working_hrs(room, working_hrs) == MTM_INVALID_PARAMETER) {
        return MTM_INVALID_PARAMETER;
    }
    if (!check_email(email)) {
        return MTM_INVALID_PARAMETER;
    }
    room->id = id;
    room->price = price;
    room->num_ppl = num_ppl;
    room->difficulty = difficulty;
    room->email = malloc(strlen(email) + 1);
    if (room->email == NULL) {
        return MTM_OUT_OF_MEMORY;
    }
    strcpy(room->email, email);
    return MTM_SUCCESS;
}

//Creates a copy for the room and returns it in the dedicated parameter.
MtmErrorCode escape_room_copy(EscapeRoom new_room, EscapeRoom original_room)
{
    if (new_room == NULL || original_room == NULL) {
        return MTM_NULL_PARAMETER;
    }
    char* room_work = get_room_working_hrs(original_room);
    MtmErrorCode code = initialize_escape_room(new_room, original_room->email
            , original_room->id, original_room->price, original_room->num_ppl
            , room_work, original_room->difficulty);

    //The result of get_room_working_hrs is dynamically allocated, so after use
    // it should be deallocated
    free(room_work);

    if (code == MTM_OUT_OF_MEMORY) {
        return MTM_OUT_OF_MEMORY;
    }
    //A set of orders is created upon creation of new_room. Now it should point
    //to the copy of the set of the original room, so the previous one should
    //be freed
    setDestroy(new_room->OrdersSet);

    new_room->OrdersSet = setCopy(original_room->OrdersSet);
    if (new_room->OrdersSet == NULL) {
        return MTM_OUT_OF_MEMORY;
    }
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

int escape_room_get_price(EscapeRoom room)
{
    assert(room != NULL);
    assert(room->price > 0);
    assert(room->price % 4 == 0);
    return room->price;
}

int escape_room_get_num_ppl(EscapeRoom room)
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

int escape_room_get_difficulty(EscapeRoom room)
{
    assert(room != NULL);
    assert(room->difficulty >=1 && room->difficulty <= 10);
    return room->difficulty;
}

//Calculetes the recommended value accrording to the formula
int escape_room_calculate_recommended_value(EscapeRoom room, int skill_level,
                                            int num_ppl)
{
    if (room == NULL) {
        return -1;
    }
    return ((room->num_ppl - num_ppl)*(room->num_ppl - num_ppl)) +
           ((room->difficulty - skill_level)*(room->difficulty - skill_level));
}

//Finds the soonest time when the room will be available
MtmErrorCode escape_room_find_closest_time(EscapeRoom room, int *day, int *hour)
{
    if (room == NULL) {
        return MTM_NULL_PARAMETER;
    }
    int free_day = 0, free_hour = room->open_hour, i;

    /* Check for all legal times, starting from the soonest, if there is an
     * order booked for this time. As the number of orders is finite, the loop
     * will run until a hour after all the orders is reached at most.
     */
    while (true) {
        //Check all the orders in the set for their time of booking
        //If
        Order order = setGetFirst(room->OrdersSet);
        for (i = 0; i < setGetSize(room->OrdersSet); i++) {

            //If you've found an order for this time, exit the loop.
            if (order_get_day(order) == free_day &&
                order_get_hour(order) == free_hour) {
                free_hour++;
                break;
            }
        }
        //If the loop reached its end, then no order is booked for this time
        if (i == setGetSize(room->OrdersSet)) {
            break;
        }
        if (free_hour == room->close_hour) {
            free_hour = room->open_hour;
            free_day++;
        }
    }
    *day = free_day;
    *hour = free_hour;
    return MTM_SUCCESS;
}

//Adds an order to the room.
MtmErrorCode escape_room_add_order(EscapeRoom room, Order order)
{
    if (room == NULL || order == NULL) {
        return MTM_NULL_PARAMETER;
    }
    int order_time = order_get_hour(order);
    if (order_time < room->open_hour || order_time >= room->close_hour) {
        return MTM_ROOM_NOT_AVAILABLE;
    }
    if (escape_room_colliding_times(room, order)) {
        return MTM_ROOM_NOT_AVAILABLE;
    }
    if (setAdd(room->OrdersSet, order) == SET_OUT_OF_MEMORY) {
        return MTM_OUT_OF_MEMORY;
    }
    return MTM_SUCCESS;
}

//Removes the order from the room.
MtmErrorCode escape_room_remove_order(EscapeRoom room, Order order)
{
    if (room == NULL || order == NULL) {
        return MTM_NULL_PARAMETER;
    }
    if (setRemove(room->OrdersSet, order) == SET_ITEM_DOES_NOT_EXIST) {
        return MTM_INVALID_PARAMETER;
    }
    return MTM_SUCCESS;
}

//Checks if at least one order exists in the room
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

//Updates the time of all the orders of the room after a day passed
MtmErrorCode escape_room_day_passed(EscapeRoom room)
{
    if (room == NULL) {
        return MTM_NULL_PARAMETER;
    }
    int orders_num = setGetSize(room->OrdersSet);
    Order order = setGetFirst(room->OrdersSet);
    for (int i = 0; i < orders_num; i++) {
        order_day_passed(order);
        order = setGetNext(room->OrdersSet);
    }
    return MTM_SUCCESS;
}

//Destroys the room
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
