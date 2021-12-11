#include "days.h"

typedef struct octopus_t {
    int energy_level;
    int last_flash;
} octopus_t;

octopus_t* create_octopus(int energy_level) {
    octopus_t* octopus = malloc(sizeof(octopus_t));
    memset(octopus, 0, sizeof(octopus_t));

    octopus->last_flash = -1;
    octopus->energy_level = energy_level;

    return octopus;
}

void print_octopus_array(octopus_t*** array, int rows, int cols, int current_step) {
#if LOG_LEVEL >= LOG_DEBUG
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            octopus_t* octopus = *(*(array + i) + j);

            bool flashed = octopus->last_flash == current_step;
            printf("%s%d%s", flashed ? "\033[33m" : "", octopus->energy_level, flashed ? "\033[0m" : "");
        }

        printf("\n");
    }

    printf("\n");
#endif
}

int flash_octopus(octopus_t*** array, int rows, int cols, octopus_t* octopus, int r, int c, int step) {
    int flash_count = 0;

    if(octopus->energy_level > 9 && octopus->last_flash != step) {
        flash_count++;

        octopus->energy_level = 0;
        octopus->last_flash = step;

        for(int i = -1; i < 2; i++) {
            for(int j = -1; j < 2; j++) {
                int nr = r + i;
                int nc = c + j;

                if(nr < 0 || nc < 0 || nr >= rows || nc >= cols) {
                    continue;
                }

                octopus_t* neighbor = *(*(array + nr) + nc);

                if(neighbor->last_flash != step) {
                    neighbor->energy_level++;
                }

                flash_count += flash_octopus(array, rows, cols, neighbor, nr, nc, step);
            }
        }
    }

    return flash_count;
}

DAY_FUNC(day_11) {
    DAY_BOILERPLATE("Day 11", 11, "in_11.txt")

    octopus_t*** octopus_array = malloc(sizeof(octopus_t**) * file_lines_count);
    memset(octopus_array, 0, sizeof(octopus_t**) * file_lines_count);

    unsigned long octopus_array_rows = file_lines_count;
    unsigned long octopus_array_cols = strcspn(*file_lines, " \r\n");

    for(unsigned long i = 0; i < file_lines_count; i++) {
        char* line = *(file_lines + i);
        unsigned long line_length = strcspn(line, " \r\n");

        octopus_t** row = malloc(sizeof(octopus_t*) * line_length);
        memset(row, 0, sizeof(octopus_t*) * line_length);

        for(unsigned long j = 0; j < line_length; j++) {
            char c = *(line + j);
            int val = c - '0';

            *(row + j) = create_octopus(val);
        }

        *(octopus_array + i) = row;
    }

    print_octopus_array(octopus_array, (int) octopus_array_rows, (int) octopus_array_cols, 0);

    unsigned long total_flashes_until_100 = 0;
    int step_all_flash = -1;

    int i = 0;

    do {
        for(int j = 0; j < octopus_array_rows; j++) {
            for(int k = 0; k < octopus_array_cols; k++) {
                octopus_t* octopus = *(*(octopus_array + j) + k);

                if(octopus->last_flash != i) {
                    octopus->energy_level++;
                }
            }
        }

        for(int j = 0; j < octopus_array_rows; j++) {
            for(int k = 0; k < octopus_array_cols; k++) {
                octopus_t* octopus = *(*(octopus_array + j) + k);

                int flashes_step = flash_octopus(octopus_array, (int) octopus_array_rows, (int) octopus_array_cols, octopus, j, k, i);
                total_flashes_until_100 += i < 100 ? flashes_step : 0;
            }
        }

        if(step_all_flash < 0) {
            bool all_flashed = true;
            for (int j = 0; j < octopus_array_rows; j++) {
                for (int k = 0; k < octopus_array_cols; k++) {
                    octopus_t* octopus = *(*(octopus_array + j) + k);
                    all_flashed &= octopus->last_flash == i;
                }
            }

            if(all_flashed) {
                step_all_flash = i + 1;
            }
        }

        if(i == 99) {
            print_octopus_array(octopus_array, (int) octopus_array_rows, (int) octopus_array_cols, i);
        }

        i++;
    } while(step_all_flash == -1);

    print_octopus_array(octopus_array, (int) octopus_array_rows, (int) octopus_array_cols, step_all_flash - 1);

    LOG("Day 11", "Total flashes after 100 steps: " UNDERLINE("%lu"), total_flashes_until_100);
    LOG("Day 11", "First step when all flash: " UNDERLINE("%d"), step_all_flash);

    for(unsigned long j = 0; j < octopus_array_rows; j++) {
        octopus_t** row = *(octopus_array + j);

        for(unsigned long k = 0; k < octopus_array_cols; k++) {
            free(*(row + k));
        }

        free(row);
    }

    free(octopus_array);

    DAY_FUNC_END
}
