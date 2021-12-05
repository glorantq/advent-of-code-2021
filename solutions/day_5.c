#include "days.h"

typedef struct line_segment_t {
    int x1, x2, y1, y2;
} line_segment_t;

line_segment_t* create_line_segment(int x1, int y1, int x2, int y2) {
    line_segment_t* line_segment = malloc(sizeof(line_segment_t));
    memset(line_segment, 0, sizeof(line_segment_t));

    line_segment->x1 = x1;
    line_segment->x2 = x2;
    line_segment->y1 = y1;
    line_segment->y2 = y2;

    return line_segment;
}

void debug_print_plot(int** arr, int rows, int cols) {
#ifndef NDEBUG
    if(cols > 10 || rows > 10) {
        WARN("Day 5", "You really shouldn't run this with the actual input...");
        return;
    }

    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            int val = *(*(arr + i) + j);

            if(val > 0) {
                printf("%d", val);
            } else {
                printf(".");
            }
        }

        printf("\n");
    }
#endif
}

void plot_line(int** array, line_segment_t* line_segment) {
    int x_mul = line_segment->x1 > line_segment->x2 ? -1 : 1;
    int y_mul = line_segment->y1 > line_segment->y2 ? -1 : 1;

    if(line_segment->x1 == line_segment->x2 || line_segment->y1 == line_segment->y2) {
        for (int j = line_segment->x1; j != line_segment->x2 + x_mul; j += x_mul) {
            for (int k = line_segment->y1; k != line_segment->y2 + y_mul; k += y_mul) {
                (*(*(array + k) + j))++;
            }
        }
    } else {
        int x_dist = (line_segment->x2 - line_segment->x1) * x_mul + 1;
        int y_dist = (line_segment->y2 - line_segment->y1) * y_mul + 1;

        int dist = x_dist > y_dist ? x_dist : y_dist;

        for(int i = 0; i < dist; i++) {
            int px = line_segment->x1 + x_mul * i;
            int py = line_segment->y1 + y_mul * i;

            (*(*(array + py) + px))++;
        }
    }
}

int get_two_overlap(int** array, int rows, int cols) {
    int two_overlap = 0;

    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            if(*(*(array + i) + j) > 1) {
                two_overlap++;
            }
        }

        memset(*(array + i), 0, sizeof(int) * cols);
    }

    return two_overlap;
}

DAY_FUNC(day_5) {
    DAY_BOILERPLATE("Day 5", 5, "in_5.txt")

    line_segment_t** line_segments = malloc(sizeof(line_segment_t*) * file_lines_count);
    memset(line_segments, 0, sizeof(line_segment_t*) * file_lines_count);

    unsigned int line_segments_used = 0;
    for(unsigned long i = 0; i < file_lines_count; i++) {
        char* line = *(file_lines + i);

        int x1 = 0;
        int y1 = 0;
        int x2 = 0;
        int y2 = 0;

        sscanf(line, "%d,%d -> %d,%d", &x1, &y1, &x2, &y2);
        *(line_segments + (line_segments_used++)) = create_line_segment(x1, y1, x2, y2);
    }

    DEBUG("Day 5", "Read %d line segments...", line_segments_used);

    int max_x = 0;
    int max_y = 0;

    for(unsigned long i = 0; i < line_segments_used; i++) {
        line_segment_t* line_segment = *(line_segments + i);

        if(line_segment->y1 > max_y) {
            max_y = line_segment->y1;
        }

        if(line_segment->y2 > max_y) {
            max_y = line_segment->y2;
        }

        if(line_segment->x1 > max_x) {
            max_x = line_segment->x1;
        }

        if(line_segment->x2 > max_x) {
            max_x = line_segment->x2;
        }
    }

    int plot_row_count = max_x + 1;
    int plot_col_count = max_y + 1;

    DEBUG("Day 5", "maxX: %d, maxY: %d, arr: %dx%d", max_x, max_y, plot_row_count, plot_col_count);

    int** plot_row_array = malloc(sizeof(int*) * plot_row_count);
    memset(plot_row_array, 0, sizeof(int*) * plot_row_count);

    for(int i = 0; i < plot_row_count; i++) {
        int* row = malloc(sizeof(int) * plot_col_count);
        memset(row, 0, sizeof(int) * plot_col_count);

        *(plot_row_array + i) = row;
    }

    for(int i = 0; i < line_segments_used; i++) {
        line_segment_t* line_segment = *(line_segments + i);

        if(line_segment->x1 == line_segment->x2 || line_segment->y1 == line_segment->y2) {
            plot_line(plot_row_array, line_segment);
        }
    }

    debug_print_plot(plot_row_array, plot_row_count, plot_col_count);

    LOG("Day 5", "At least two overlap: " UNDERLINE("%d"), get_two_overlap(plot_row_array, plot_row_count, plot_col_count));

    for(int i = 0; i < line_segments_used; i++) {
        line_segment_t* line_segment = *(line_segments + i);

        plot_line(plot_row_array, line_segment);
    }

    debug_print_plot(plot_row_array, plot_row_count, plot_col_count);

    LOG("Day 5", "At least two overlap (diagonals): " UNDERLINE("%d"), get_two_overlap(plot_row_array, plot_row_count, plot_col_count));

    for(int i = 0; i < plot_col_count; i++) {
        free(*(plot_row_array + i));
    }

    free(plot_row_array);

    for(unsigned long i = 0; i < line_segments_used; i++) {
        free(*(line_segments + i));
    }

    free(line_segments);
    DAY_FUNC_END
}
