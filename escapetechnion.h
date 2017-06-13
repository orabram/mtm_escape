#ifndef ESCAPETECHNION_H_
#define ESCAPETECHNION_H_

#include <stdio.h>
#include <stdbool.h>
#include "company.h"
#include "customer.h"
#include "list.h"

typedef struct escapetechnion *EscapeTechnion;


EscapeTechnion create_escapetechnion();

MtmErrorCode escapetechnion_set_output_channel(EscapeTechnion escape,
                                               FILE* output_channel);

//MtmErrorCode initialize_escapetechnion();

MtmErrorCode escapetechnion_add_company(EscapeTechnion escape,
                                        char* email, TechnionFaculty faculty);

MtmErrorCode escapetechnion_remove_company(EscapeTechnion escape, char* email);

MtmErrorCode escapetechnion_add_room(EscapeTechnion escape, char* email,
                                     int id, int price, int num_ppl,
                                     char* working_hrs, int difficulty);

MtmErrorCode escapetechnion_remove_room(EscapeTechnion escape,
                                        TechnionFaculty faculty, int id);

MtmErrorCode escapetechnion_add_customer(EscapeTechnion escape, char* email,
                                         TechnionFaculty faculty,
                                         int skill_level);

MtmErrorCode escapetechnion_remove_customer(EscapeTechnion escape, char* email);

MtmErrorCode escapetechnion_create_order(EscapeTechnion escape, char* email,
                                         TechnionFaculty faculty, int id,
                                         char* time, int num_ppl);

MtmErrorCode escapetechnion_recommended_room(EscapeTechnion escape, char* email,
                                             int num_ppl);

MtmErrorCode escapetechnion_reportday(EscapeTechnion escape);

void escapetechnion_reportbest(EscapeTechnion escape);

void escapetechnion_destroy(EscapeTechnion escape);

#endif