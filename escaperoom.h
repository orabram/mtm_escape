#ifndef ESCAPEROOM_H_
#define ESCAPEROOM_H_

#include <stdio.h>
#include <stdbool.h>
#include "set.h"
#include "order.h"

typedef struct escaperoom{
    char* email;
    int id;
    int price;
    int num_ppl;
    int working_hrs;
    int difficulty;
    Set room_orders;
} *EscapeRoom;

MtmErrorCode create_escape_room()

