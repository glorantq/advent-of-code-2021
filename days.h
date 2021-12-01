#ifndef ADVENT_OF_CODE_DAYS_H
#define ADVENT_OF_CODE_DAYS_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "log.h"

typedef struct execution_info_t {
    bool has_parameter;
    char* program_parameter;
} execution_info_t;

void day_1(execution_info_t* execution_info);
#endif
