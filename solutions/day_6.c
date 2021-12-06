#include "days.h"

#define NUM_DAYS 256

void debug_print_timers(const long* timers) {
    for(int i = 0; i < 9; i++) {
        DEBUG("Day 6", "%d: %li", i, *(timers + i));
    }
}

DAY_FUNC(day_6) {
    DAY_BOILERPLATE("Day 6", 6, "in_6.txt")

    long* fish_timers = malloc(sizeof(long) * 10);
    memset(fish_timers, 0, sizeof(long) * 10);

    char* line = *file_lines;
    char* token = strtok(line, ",");
    while(token != NULL) {
        long val = strtol(token, NULL, 10);
        (*(fish_timers + val))++;

        token = strtok(NULL, ",");
    }

    debug_print_timers(fish_timers);

    for(int i = 0; i < NUM_DAYS; i++) {
        (*(fish_timers + 7)) += (*fish_timers);
        (*(fish_timers + 9)) = (*fish_timers);
        *fish_timers = 0;

        for(int j = 1; j < 9; j++) {
            (*(fish_timers + (j - 1))) = (*(fish_timers + j));
        }

        *(fish_timers + 8) = *(fish_timers + 9);
    }

    DEBUG("Day 6", "After calculations:");
    debug_print_timers(fish_timers);

    unsigned long fish_sum = 0;
    for(int i = 0; i < 9; i++) {
        fish_sum += *(fish_timers + i);
    }

    LOG("Day 6", "Fish count: " UNDERLINE("%lu"), fish_sum);

    free(fish_timers);
    DAY_FUNC_END
}
