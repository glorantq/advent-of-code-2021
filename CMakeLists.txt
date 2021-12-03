cmake_minimum_required(VERSION 3.16)
project(advent_of_code DESCRIPTION "Advent of Code 2021" HOMEPAGE_URL "https://glorantv.web.elte.hu/" LANGUAGES "C")

set(CMAKE_C_STANDARD 99)

add_executable(advent_of_code main.c log.h solutions/days.h solutions/day_1.c solutions/day_2.c solutions/day_3.c util/files.c util/files.h solutions/day_4.c)
target_compile_options(advent_of_code PRIVATE -ansi -Wall -Wpedantic -Wno-unknown-pragmas -Werror)