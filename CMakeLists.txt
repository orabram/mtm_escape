cmake_minimum_required(VERSION 3.7)
project(mtm_escape)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -pedantic-errors -Werror")
set(SOURCE_FILES main.c customer.h customer.c
        list.h list_mtm1.h mtm_ex3.h set.h
        order.h order.c escaperoom.c escaperoom.h
        company.c company.h escapetechnion.c escapetechnion.h company_test.c escaperoom_test.c)
add_executable(mtm_escape ${SOURCE_FILES})
target_link_libraries(mtm_escape ${CMAKE_SOURCE_DIR}/libmtm.a)
