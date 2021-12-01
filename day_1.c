#include "days.h"

void day_1(execution_info_t* execution_info) {
    char* file_name = "in_1.txt";
    if(execution_info->has_parameter) {
        file_name = execution_info->program_parameter;
    }

    FILE* in_file = fopen(file_name, "r");
    if(in_file == NULL) {
        ERR("Day 1", "Failed to open file!");
        return;
    }

    int array_size = sizeof(int) * 8;
    int array_used = 0;
    int* in_depths = malloc(array_size);

    unsigned long line_size = sizeof(int) * 10uL;
    char* line_read = malloc(line_size);

    while (getline(&line_read, &line_size, in_file) != EOF) {
        if(array_used >= array_size / sizeof(int)) {
            array_size *= 2;

            int* new_depths = realloc(in_depths, array_size);
            if(new_depths == NULL) {
                ERR("Day 1", "Failed to re-allocate array!");
                return;
            }

            in_depths = new_depths;
        }

        char* depth_remainder;
        int depth_converted = (int) strtol(line_read, &depth_remainder, 10);

        if(strlen(depth_remainder) == 0 || (strlen(depth_remainder) == 1 && depth_remainder[0] == '\n')) {
            in_depths[array_used++] = depth_converted;
        } else {
            ERR("Day 1", "Failed to parse number!");
        }
    }

    DEBUG_("Day 1", "Read %d depths", array_used);

    int increase_count = 0;
    for(int i = 1; i < array_used; i++) {
        int depth_0 = in_depths[i - 1];
        int depth_1 = in_depths[i];

        if(depth_1 > depth_0) {
            increase_count++;
        }
    }

    LOG_("Day 1", "Depth increase count: " UNDERLINE("%d"), increase_count);

    free(line_read);

    increase_count = 0;
    int previous_sum = in_depths[0] + in_depths[1] + in_depths[2];

    for(int i = 0; i < array_used - 2; i++) {
        int depth_1 = in_depths[i];
        int depth_2 = in_depths[i + 1];
        int depth_3 = in_depths[i + 2];

        int sum = depth_1 + depth_2 + depth_3;
        if(sum > previous_sum) {
            increase_count++;
        }

        previous_sum = sum;
    }

    LOG_("Day 1", "Depth increase count (3-sum): " UNDERLINE("%d"), increase_count);

    free(in_depths);
    fclose(in_file);
}