#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <string.h>
#include "escapetechnion.h"

struct escapetechnion{
    Set CompanySet;
    Set CustomersSet;
    Set CustomerEmailsSet;
    Set CompanyEmailsSet;
    int faculties[FACULTIES_NUM];
};

static

EscapeTechnion create_escapetechnion(){
    EscapeTechnion escape = malloc(sizeof(EscapeTechnion));
    if(escape == NULL)
    {
        return NULL;
    }
    escape->CompanySet = setCreate(company_copy, company_destroy, company_compare);
    if (escape->CompanySet == NULL)
    {
        return NULL;
    }
    escape->CustomersSet = setCreate(customer_copy, customer_destroy, customer_compare);
    if(escape->CustomersSet == NULL)
    {
        return NULL;
    }
    escape->CompanyEmailsSet = setCreate(strcpy, free , strcmp);
    if(escape->CompanyEmailsSet == NULL) {
        return NULL;
    }
    escape->CustomerEmailsSet = setCreate(strcpy, free, strcmp);
    if(escape->CustomerEmailsSet == NULL)
    {
        return NULL;
    }
    return escape;
}

MtmErrorCode escapetechnion_add_company(EscapeTechnion escape, char* email,
                                        TechnionFaculty faculty)
{
    Company newcomp = create_company();
    if(newcomp == NULL)
    {
        return MTM_OUT_OF_MEMORY;
    }
    MtmErrorCode code = initialize_company(newcomp, email, faculty);
    if(code != MTM_SUCCESS)
    {
        return code;
    }
    setAdd(escape->CompanySet, newcomp);
    setAdd(escape->CompanyEmailsSet, email);
    return MTM_SUCCESS;
}

MtmErrorCode escapetechnion_destroy_company(EscapeTechnion escape, char* email)
{
    if(!setIsIn(escape->CompanyEmailsSet, email))
    {
        return MTM_COMPANY_EMAIL_DOES_NOT_EXIST;
    }
    Company tempcomp = create_company();
    setRemove(escape->CompanySet, email);
    if(tempcomp.)
}