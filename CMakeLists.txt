cmake_minimum_required(VERSION 3.7)
project(mtm_escape)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -pedantic-errors -Werror")
set(SOURCE_FILES escapetechnion.c escaperoom.c company.c
        customer.c order.c mtm_escape.c)
add_executable(mtm_escape ${SOURCE_FILES})
target_link_libraries(mtm_escape ${CMAKE_SOURCE_DIR}/libmtm32.a)
