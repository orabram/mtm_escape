cmake_minimum_required(VERSION 3.7)
project(mtm_escape)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c customer.h customer.c
        list.h list_mtm1.h mtm_ex3.h set.h
        order.h order.c escaperoom.c escaperoom.h
        company.c company.h)
add_executable(mtm_escape ${SOURCE_FILES})
