cmake_minimum_required(VERSION 3.16)
project(advent_of_code DESCRIPTION "Advent of Code 2021" HOMEPAGE_URL "https://glorantv.web.elte.hu/" LANGUAGES "C")

set(CMAKE_C_STANDARD 99)

set(solution_names "")
set(solution_definitions "")
FILE(GLOB solution_files "${CMAKE_CURRENT_SOURCE_DIR}/solutions/*.c")
foreach(solution_file ${solution_files})
    get_filename_component(solution_name ${solution_file} NAME_WLE)
    string(APPEND solution_names "${solution_name}, ")
    string(APPEND solution_definitions "void ${solution_name} CMAKE_TYPE_HACK CMAKE_SEMICOLON_HACK ")
endforeach()
string(LENGTH ${solution_names} names_length)
math(EXPR names_length "${names_length} - 2")
string(SUBSTRING ${solution_names} 0 ${names_length} solution_names)

message(${solution_definitions})

add_executable(advent_of_code main.c log.h util/files.c util/files.h solutions/days.h ${solution_files})
target_compile_options(advent_of_code PRIVATE -ansi -Wall -Wpedantic -Wno-unknown-pragmas -Werror)

add_compile_definitions(SOLUTION_METHOD_NAMES=${solution_names} SOLUTION_METHOD_DEFINITIONS=${solution_definitions})