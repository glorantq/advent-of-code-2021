#ifndef ADVENT_OF_CODE_DAYS_H
#define ADVENT_OF_CODE_DAYS_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../log.h"

#define DAY_FUNC(name) void name (execution_info_t* execution_info)

typedef struct execution_info_t {
    bool has_parameter;
    char* program_parameter;
} execution_info_t;

DAY_FUNC(day_1);
DAY_FUNC(day_2);

#endif
