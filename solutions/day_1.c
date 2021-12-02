#include "days.h"

DAY_FUNC(day_1) {
    OPEN_DAY_INPUT(1, "in_1.txt");
    READ_DAY_INPUT(in_file);

    if(file_lines == NULL) {
        ERR("Day 1", "Failed to read input file!");
        return;
    }

    unsigned long array_size = sizeof(int) * file_lines_count;
    int* in_depths = malloc(array_size);

    for(unsigned long i = 0; i < file_lines_count; i++) {
        char* line = *(file_lines + i);

        char* depth_remainder;
        int depth_converted = (int) strtol(line, &depth_remainder, 10);

        if(strlen(depth_remainder) == 0 || (strlen(depth_remainder) == 1 && depth_remainder[0] == '\n')) {
            in_depths[i] = depth_converted;
        } else {
            ERR("Day 1", "Failed to parse number!");
        }
    }

    DEBUG_("Day 1", "Read %lu depths", file_lines_count);
    free_file_lines(file_lines, file_lines_count);

    int increase_count = 0;
    for(int i = 1; i < file_lines_count; i++) {
        int depth_0 = in_depths[i - 1];
        int depth_1 = in_depths[i];

        if(depth_1 > depth_0) {
            increase_count++;
        }
    }

    LOG_("Day 1", "Depth increase count: " UNDERLINE("%d"), increase_count);


    increase_count = 0;
    int previous_sum = in_depths[0] + in_depths[1] + in_depths[2];

    for(int i = 0; i < file_lines_count - 2; i++) {
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