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

int company_compare(Company comp1, Company comp2);

//Adds a room to the company.
MtmErrorCode company_add_room(Company comp, EscapeRoom escape);

//Removes a room from the company.
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
                              int* id, int* time);

bool company_room_got_orders(Company comp, int id);

bool company_got_orders(Company comp);

void company_destroy(Company comp);

#endif