#include "days.h"

DAY_FUNC(day_4) {
    OPEN_DAY_INPUT(4, "in_4.txt")
    READ_DAY_INPUT(in_file);

    if(file_lines == NULL) {
        ERR("Day 4", "Failed to read input file!");
        fclose(in_file);
        return;
    }

    free_file_lines(file_lines, file_lines_count);
    fclose(in_file);
}