#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "escapetechnion.h"

struct escapetechnion{
    List CompanyList;
    Set CustomersSet;
    Set CustomerEmailsSet;
    Set CompanyEmailsSet;
    int faculties[FACULTIES_NUM];
};

EscapeTechnion create_escapetechnion(){
    EscapeTechnion escape = malloc(sizeof(EscapeTechnion));
    if(escape == NULL)
    {
        return NULL;
    }
    escape->CompanyList = listCreate(company_copy, company_destroy);
    if (escape->CompanyList == NULL)
    {
        return NULL;
    }
    escape->CustomersSet = setClear()
    return escape;
}

MtmErrorCode initi