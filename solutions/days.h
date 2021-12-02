#ifndef ADVENT_OF_CODE_DAYS_H
#define ADVENT_OF_CODE_DAYS_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../log.h"
#include "../util/files.h"

#define OPEN_DAY_INPUT(day_n, file_name_param) char* file_name = file_name_param; \
                                         if(execution_info->parameter_count > (day_n)) { file_name = execution_info->program_parameters[(day_n) - 1];  } \
                                         FILE* in_file = fopen(file_name, "r"); \
                                         if(in_file == NULL) { ERR("OPEN_DAY_INPUT", "Failed to open input file!"); return; }

#define READ_DAY_INPUT(file) unsigned long file_lines_count = 0; \
                                  char** file_lines = read_file_lines(file, &file_lines_count)

#define DAY_FUNC(name) void name (execution_info_t* execution_info)

typedef struct execution_info_t {
    int parameter_count;
    char** program_parameters;
} execution_info_t;

DAY_FUNC(day_1);
DAY_FUNC(day_2);
DAY_FUNC(day_3);

#endif
