#include <limits.h>

#include "days.h"
#include "../util/strings.h"
#include "../util/math.h"

int sum_n_halt(int n, int acc, int halt) {
    if(n == 0) {
        return 0;
    }

    if(acc >= halt) {
        return 0;
    }

    acc += n;

    return n + sum_n_halt(n - 1, acc, halt);
}

DAY_FUNC(day_7) {
    // Processing this in debug mode is slow, but fine in release mode. I don't want to wait for this while I do other days.
    #ifdef NDEBUG
        #define FILE_NAME "in_7.txt";
    #else
        #define FILE_NAME "in_7_test.txt";
    #endif

    DAY_BOILERPLATE("Day 7", 7, FILE_NAME)

    int crabs_count = count_character(*file_lines, ',') + 1;
    int crabs_count_used = 0;
    int* crabs_positions = malloc(sizeof(int) * crabs_count);
    memset(crabs_positions, 0, sizeof(int) * crabs_count);

    char* token = strtok(*file_lines, ",");
    while(token != NULL) {
        *(crabs_positions + (crabs_count_used++)) = (int) strtol(token, NULL, 10);
        token = strtok(NULL, ",");
    }

    DEBUG("Day 7", "Read %d crab positions...", crabs_count_used);

    int max_position = INT_MIN;
    for(int i = 0; i < crabs_count_used; i++) {
        max_position = MAX(max_position, *(crabs_positions + i));
    }

    int min_fuel = INT_MAX;
    int min_fuel_fac = INT_MAX;

    for(int i = 0; i < max_position; i++) {
        int used_fuel = 0;
        int used_fuel_fac = 0;

        for(int j = 0; j < crabs_count_used; j++) {
            int crab_pos = *(crabs_positions + j);
            used_fuel += ABS(crab_pos - i);
            used_fuel_fac += sum_n_halt(ABS(crab_pos - i), 0, min_fuel_fac);

            if(used_fuel >= min_fuel && used_fuel_fac >= min_fuel_fac) {
                break;
            }
        }

        if(used_fuel < min_fuel) {
            min_fuel = used_fuel;
        }

        if(used_fuel_fac < min_fuel_fac) {
            min_fuel_fac = used_fuel_fac;
        }
    }

    LOG("Day 7", "Min. fuel needed: " UNDERLINE("%d"), min_fuel);
    LOG("Day 7", "Min. fuel needed (sum_n): " UNDERLINE("%d"), min_fuel_fac);

    free(crabs_positions);
    DAY_FUNC_END
}
