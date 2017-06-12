#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "company.h"

#define ASSERT_TEST(b) do { \
        if (!(b)) { \
                printf("\nAssertion failed at %s:%d %s\n",__FILE__,__LINE__,#b); \
                return false; \
        } \
} while (0)



int main()
{
    Company comp  = create_company();
    assert(comp != NULL);
    initialize_company(comp, "hello@world.com", COMPUTER_SCIENCE);



    return 0;
}
