#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "log.h"
#include "solutions/days_def.h"

int main(int argc, char** argv) {
    execution_info_t *execution_info = malloc(sizeof(execution_info_t));
    memset(execution_info, 0, sizeof(execution_info_t));

    if (argc > 1) {
        execution_info->parameter_count = argc - 1;
        execution_info->program_parameters = argv + 1;
    } else {
        WARN("AdventOfCode-C", "No parameter specified! Using defaults...");
    }

    void (*methods[])(execution_info_t*) = {
            day_1, day_2, day_3, day_4, day_5,
            day_6, day_7, day_8, day_9, day_10,
            day_11, day_12, day_13, day_14
    };

    int methods_size = sizeof(methods) / sizeof(void (*)(execution_info_t*));

#ifdef NDEBUG
    double time_sum = 0.;
#endif

    double* method_times = malloc(sizeof(double) * methods_size);
    memset(method_times, 0, sizeof(double) * methods_size);

    for (int i = 0; i < methods_size; i++) {
        clock_t start_time;
        clock_t end_time;

        start_time = clock();
        methods[i](execution_info);
        end_time = clock();

        double time_diff = (double) (end_time - start_time) / CLOCKS_PER_SEC * 1000;
        *(method_times + i) = time_diff;
#ifdef NDEBUG
        time_sum += time_diff;
#endif
    }

    SUCCESS("AdventOfCode-C", "🎄 Complete");

#ifdef NDEBUG
    printf("\n");
    double time_max = -1;
    int max_idx = 0;
    double time_min = 99;
    int min_idx = 0;
    SUCCESS("AdventOfCode-C", "==================================");
    SUCCESS("AdventOfCode-C", "Day timings: ");

    for(int i = 0; i < methods_size; i++) {
        double time = *(method_times + i);
        if (time < time_min) {
            time_min = time;
            min_idx = i;
        }

        if (time > time_max) {
            time_max = time;
            max_idx = i;
        }
    }

    for(int i = 0; i < methods_size; i++) {
        double time = *(method_times + i);

        SUCCESS("AdventOfCode-C", "    Day %d: %s%.3fms", i + 1, time >= time_max ? "\033[31m" : (time >= (time_max - time_min) / 2. ? "\033[33m" : "\033[32m"), time);
    }

    SUCCESS("AdventOfCode-C", "Total time used: %.3fms", time_sum);
    SUCCESS("AdventOfCode-C", "Average time used: %.3fms", time_sum / (double) methods_size);
    SUCCESS("AdventOfCode-C", "Max: %.3fms (Day %d), Min: %.3fms (Day %d)", time_max, max_idx + 1, time_min, min_idx + 1);
    SUCCESS("AdventOfCode-C", "==================================");
#endif

    free(method_times);
    free(execution_info);
    return 0;
}
