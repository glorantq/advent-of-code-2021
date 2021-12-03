#include "solutions/days.h"

int main(int argc, char** argv) {
    execution_info_t* execution_info = malloc(sizeof(execution_info_t));
    memset(execution_info, 0, sizeof(execution_info_t));

    if(argc > 1) {
        execution_info->parameter_count = argc - 1;
        execution_info->program_parameters = argv + 1;
    } else {
        WARN("AdventOfCode-C", "No parameter specified! Using defaults...");
    }

    void (*methods[])(execution_info_t*) = {
        SOLUTION_METHOD_NAMES
    };

    int methods_size = sizeof(methods) / sizeof(void (*)(execution_info_t*));
    for(int i = 0; i < methods_size; i++) {
        methods[i](execution_info);
    }

    SUCCESS("AdventOfCode-C", "🎄 Complete");

    free(execution_info);
    return 0;
}
