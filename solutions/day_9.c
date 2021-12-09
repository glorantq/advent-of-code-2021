#include "days.h"
#include "limits.h"

int get_number(int** array, unsigned long row, unsigned long row_size, unsigned long col, unsigned long col_size) {
    if(row < 0 || col < 0) {
        return INT_MAX;
    }

    if(row >= row_size || col >= col_size) {
        return INT_MAX;
    }

    return *(*(array + row) + col);
}

void walk_basin(int** array, int start_value, unsigned long start_row, unsigned long start_col, unsigned long row_size, unsigned long col_size) {
    if(start_row < 0 || start_col < 0 || start_row >= row_size || start_col >= col_size || start_value >= 9 || start_value == -1) {
        return;
    }

    *(*(array + start_row) + start_col) = -1;

    int val_a = get_number(array, start_row + 1, row_size, start_col, col_size);
    int val_b = get_number(array, start_row, row_size, start_col + 1, col_size);
    int val_c = get_number(array, start_row - 1, row_size, start_col, col_size);
    int val_d = get_number(array, start_row, row_size, start_col - 1, col_size);

    walk_basin(array, val_a, start_row + 1, start_col, row_size, col_size);
    walk_basin(array, val_b, start_row, start_col + 1, row_size, col_size);
    walk_basin(array, val_c, start_row - 1, start_col, row_size, col_size);
    walk_basin(array, val_d, start_row, start_col - 1, row_size, col_size);
}

int** copy_array(int** array, unsigned long row_count, unsigned long col_count) {
    int** new_array = malloc(sizeof(int*) * row_count);
    memset(new_array, 0, sizeof(int*) * row_count);

    for(unsigned long i = 0; i < row_count; i++) {
        int* row = malloc(sizeof(int) * col_count);
        memcpy(row, *(array + i), sizeof(int) * col_count);
        *(new_array + i) = row;
    }

    return new_array;
}

void free_array(int** array, unsigned long row_count) {
    for(int i = 0; i < row_count; i++) {
        free(*(array + i));
    }

    free(array);
}

int find_max_and_clear(int* array, int array_size) {
    int max = INT_MIN;
    int max_idx = -1;

    for(int i = 0; i < array_size; i++) {
        int val = *(array + i);
        if(val > max) {
            max = val;
            max_idx = i;
        }
    }

    *(array + max_idx) = INT_MIN;
    return max;
}

DAY_FUNC(day_9) {
    DAY_BOILERPLATE("Day 9", 9, "in_9.txt")

    int** number_rows = malloc(sizeof(int*) * file_lines_count);
    memset(number_rows, 0, sizeof(int*) * file_lines_count);

    unsigned long row_length = strcspn(*file_lines, " \r\n");
    for(unsigned long i = 0; i < file_lines_count; i++) {
        int* row = malloc(sizeof(int) * row_length);
        memset(row, 0, sizeof(int) * row_length);

        for(int j = 0; j < row_length; j++) {
            char c = *(*(file_lines + i) + j);
            int val = c - '0';

            *(row + j) = val;
        }

        *(number_rows + i) = row;
    }

    int basin_array_use = 0;
    int basin_array_size = 3;
    int* basin_array = malloc(sizeof(int) * basin_array_size);
    memset(basin_array, 0, sizeof(int) * basin_array_size);

    unsigned long risk_level_sum = 0;
    for(unsigned long i = 0; i < file_lines_count; i++) {
        for(unsigned long j = 0; j < row_length; j++) {
            int val = get_number(number_rows, i, file_lines_count, j, row_length);

            int val_a = get_number(number_rows, i + 1, file_lines_count, j, row_length);
            int val_b = get_number(number_rows, i, file_lines_count, j + 1, row_length);
            int val_c = get_number(number_rows, i - 1, file_lines_count, j, row_length);
            int val_d = get_number(number_rows, i, file_lines_count, j - 1, row_length);

            if(val < val_a && val < val_b && val < val_c && val < val_d) {
                risk_level_sum += val + 1;

                int** copied = copy_array(number_rows, file_lines_count, row_length);
                walk_basin(copied, val, i, j, file_lines_count, row_length);
                int basin_size = 0;

                for(int k = 0; k < file_lines_count; k++) {
                    for (int l = 0; l < row_length; l++) {
                        int _val = *(*(copied + k) + l);
                        if (_val == -1) {
                            basin_size++;
                        }
                    }
                }

                free_array(copied, file_lines_count);

                if(basin_array_use + 1 >= basin_array_size) {
                    basin_array_size *= 2;
                    basin_array = realloc(basin_array, sizeof(int) * basin_array_size);
                }

                *(basin_array + (basin_array_use++)) = basin_size;
            }
        }
    }

    LOG("Day 9", "Risk level sum: " UNDERLINE("%lu"), risk_level_sum);

    int basin_1 = find_max_and_clear(basin_array, basin_array_use);
    int basin_2 = find_max_and_clear(basin_array, basin_array_use);
    int basin_3 = find_max_and_clear(basin_array, basin_array_use);

    int basin_mul = basin_1 * basin_2 * basin_3;
    LOG("Day 9", "Basin sizes: " UNDERLINE("%d"), basin_mul);

    free(basin_array);
    free_array(number_rows, file_lines_count);
    DAY_FUNC_END
}
