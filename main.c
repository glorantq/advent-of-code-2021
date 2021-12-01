#include "days.h"

int main(int argc, char** argv) {
    execution_info_t* execution_info = malloc(sizeof(execution_info_t));
    memset(execution_info, 0, sizeof(execution_info_t));

    if(argc > 1) {
        execution_info->has_parameter = true;
        execution_info->program_parameter = argv[1];
    } else {
        WARN("AdventOfCode-C", "No parameter specified! Using defaults...");
    }

    day_1(execution_info);
    SUCCESS("AdventOfCode-C", "🎄 Complete");

    free(execution_info);
    return 0;
}
