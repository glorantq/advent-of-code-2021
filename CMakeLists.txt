cmake_minimum_required(VERSION 3.16)
project(advent_of_code DESCRIPTION "Advent of Code 2021" HOMEPAGE_URL "https://glorantv.web.elte.hu/" LANGUAGES "C")

set(CMAKE_C_STANDARD 99)

add_executable(advent_of_code main.c log.h util/files.c util/files.h solutions/days.h solutions/day_1.c solutions/day_2.c solutions/day_3.c solutions/day_4.c solutions/days_def.h solutions/day_5.c solutions/day_6.c util/math.h solutions/day_7.c util/strings.h util/strings.c solutions/day_8.c solutions/day_9.c solutions/day_10.c solutions/day_11.c solutions/day_12.c solutions/day_13.c solutions/day_14.c solutions/day_15.c)
target_compile_options(advent_of_code PRIVATE -ansi -Wall -Wpedantic -Wno-unknown-pragmas -Werror -O3)
