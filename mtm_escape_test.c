#include "escapetechnion.h"
#include "mtm_ex3.h"
#include <string.h>
int main(){
    EscapeTechnion escape = create_escapetechnion();
    MtmErrorCode code = escapetechnion_set_output_channel(escape, stdout);
    code = escapetechnion_add_company(escape, "company1@gmail", 0);
    code =escapetechnion_add_company(escape, "company2@gmail", 2);
    code =escapetechnion_add_company(escape, "company3@gmail", 0);
    code =escapetechnion_add_company(escape, "company4@gmail", 8);
    code =escapetechnion_add_company(escape, "company5@gmail", 16);
    code =    escapetechnion_add_company(escape, "company6@gmail", 6);
    code =    escapetechnion_add_company(escape, "company7@gmail", 1);
    code =    escapetechnion_add_room(escape, "company1@gmail", 2, 80, 2, "05-15", 4);
    code =    escapetechnion_add_room(escape, "company2@gmail", 1, 4, 3, "08-19", 1);
    code =    escapetechnion_add_room(escape, "company2@gmail", 4, 20, 4, "02-22", 5);
    code =    escapetechnion_add_room(escape, "company3@gmail", 5, 24, 4, "02-22", 7);
    code =    escapetechnion_add_room(escape, "company4@gmail", 2, 24, 4, "06-18", 8);
    code =    escapetechnion_add_room(escape, "company5@gmail", 8, 60, 2, "08-09", 2);
    code =    escapetechnion_add_room(escape, "company6@gmail", 11, 120, 7, "18-24", 9);
    code =    escapetechnion_add_room(escape, "company6@gmail", 14, 200, 2, "15-23", 1);
    code =     escapetechnion_add_room(escape, "company7@gmail", 12, 12, 2, "12-22", 6);
    code =    escapetechnion_add_customer(escape, "1@", 0, 1);
    code =   escapetechnion_add_customer(escape, "2@", 1, 4);
    code =    escapetechnion_add_customer(escape, "3@", 0, 1);
    code =   escapetechnion_add_customer(escape, "4@", 1, 5);
    code =   escapetechnion_add_customer(escape, "5@", 8, 1);
    code =      escapetechnion_add_customer(escape, "6@", 16, 7);
    code =      escapetechnion_create_order(escape, "1@", 0, 2, "0-10", 4);
    code =     escapetechnion_create_order(escape, "2@", 0, 2, "0-5", 4);
    code =     escapetechnion_create_order(escape, "1@", 0, 2, "1-10", 5);
    code =    escapetechnion_create_order(escape, "3@", 2, 4, "0-02", 4);
    code =    escapetechnion_create_order(escape, "3@", 2, 4, "0-03", 4);
    code =    escapetechnion_create_order(escape, "5@", 16, 8, "0-08", 5);
    code =     escapetechnion_create_order(escape, "6@", 6, 11, "1-23", 5);
    code =     escapetechnion_create_order(escape, "4@", 6, 11, "2-23", 5);
    code =    escapetechnion_remove_room(escape, 0, 2);
    code =    escapetechnion_reportday(escape);
    code = escapetechnion_remove_company(escape, "company5@gmail");
    code = escapetechnion_create_order(escape, "2@", 8, 2, "0-10", 5);
    code = escapetechnion_create_order(escape, "1@", 0, 2, "0-10", 5);
    code = escapetechnion_create_order(escape, "3@", 0, 2, "0-10", 5);
    code = escapetechnion_create_order(escape, "1@", 1, 12, "0-11", 5);
    code = escapetechnion_create_order(escape, "1@", 16, 8, "0-11", 5);
    code = escapetechnion_remove_customer(escape, "4@");
    code = escapetechnion_reportday(escape);
    code = escapetechnion_remove_customer(escape, "4@");
    escapetechnion_reportbest(escape);
    code = escapetechnion_reportday(escape);
    code = escapetechnion_recommended_room(escape, "3@", 6);
    code = escapetechnion_reportday(escape);

    return code;
}