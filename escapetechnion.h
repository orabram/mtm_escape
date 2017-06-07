#ifndef ESCAPETECHNION_H_
#define ESCAPETECHNION_H_

#include <stdio.h>
#include <stdbool.h>
#include "company.h"
#include "customer.h"
#include "list.h"

#define FACULTIES_NUM 19

typedef struct escapetechnion{
    List CompanyList;
    Set CustomersSet;
    Set CustomerEmailsSet;
    Set CompanyEmailsSet;
    int faculties[FACULTIES_NUM];
} *EscapeTechnion;


EscapeTechnion create_escapetechnion();

MtmErrorCode initialize_escapetechnion();

MtmErrorCode escapetechnion_add_company(char* email, TechnionFaculty faculty);

MtmErrorCode escapetechnion_remove_company(char* email);

MtmErrorCode escapetechnion_add_room(char* email, int id, int price,
                                     int num_ppl, char* working_hrs,
                                     int difficulty);

MtmErrorCode escapetechnion_remove_room(TechnionFaculty faculty, int id);

MtmErrorCode escapetechnion_add_customer(char* email, TechnionFaculty faculty,
                                         int skill_level);

MtmErrorCode escapetechnion_remove_customer(char* email);

MtmErrorCode escapetechnion_create_order(char* email, TechnionFaculty faculty,
                                         int id, int time, int num_ppl);

MtmErrorCode escapetechnion_recommended_room(char* email, int num_ppl);

MtmErrorCode escapetechnion_reportday();

MtmErrorCode escapetechnion_reportbest();
