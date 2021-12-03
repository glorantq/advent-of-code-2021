#ifndef ADVENT_OF_CODE_DAYS_H
#define ADVENT_OF_CODE_DAYS_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../log.h"
#include "../util/files.h"

#define OPEN_DAY_INPUT(day_n, file_name_param) char* file_name = file_name_param; \
                                         if(execution_info->parameter_count > (day_n) - 1) { file_name = execution_info->program_parameters[(day_n) - 1];  } \
                                         FILE* in_file = fopen(file_name, "r"); \
                                         if(in_file == NULL) { ERR("OPEN_DAY_INPUT", "Failed to open input file!"); return; }

#define READ_DAY_INPUT unsigned long file_lines_count = 0; \
                                  char** file_lines = read_file_lines(in_file, &file_lines_count)

#define CHECK_INPUT_READ(day_str) if(file_lines == NULL) { ERR(day_str, "Failed to read input file!"); fclose(in_file); return; }

#define DAY_BOILERPLATE(day_str, day_n, file_name_param) OPEN_DAY_INPUT(day_n, file_name_param) \
                                                            READ_DAY_INPUT; \
                                                            CHECK_INPUT_READ(day_str)

#define DAY_FUNC_END free_file_lines(file_lines, file_lines_count); fclose(in_file);

#define DAY_FUNC(name) void name (execution_info_t* execution_info)

typedef struct execution_info_t {
    int parameter_count;
    char **program_parameters;
} execution_info_t;

#define CMAKE_TYPE_HACK (execution_info_t* execution_info)
#define CMAKE_SEMICOLON_HACK ;

SOLUTION_METHOD_DEFINITIONS

#endif
