#ifndef ADVENT_OF_CODE_DAYS_DEF_H
#define ADVENT_OF_CODE_DAYS_DEF_H

typedef struct execution_info_t {
    int parameter_count;
    char **program_parameters;
} execution_info_t;

#define DAY_FUNC(name) void name (execution_info_t* execution_info)

DAY_FUNC(day_1);
DAY_FUNC(day_2);
DAY_FUNC(day_3);
DAY_FUNC(day_4);
DAY_FUNC(day_5);
DAY_FUNC(day_6);
DAY_FUNC(day_7);
DAY_FUNC(day_8);
DAY_FUNC(day_9);
DAY_FUNC(day_10);

#endif
