#include "days.h"
#include "../util/strings.h"

typedef enum direction_t {
    X, Y
} direction_t;

typedef struct fold_command_t {
    direction_t direction;
    int place;
} fold_command_t;

typedef struct point_t {
    int x;
    int y;
} point_t;

void print_point_array(point_t** array, int array_size) {
    int width = 0;
    int height = 0;

    for(int i = 0; i < array_size; i++) {
        point_t* p = *(array + i);

        if(p->x > width) {
            width = p->x;
        }

        if(p->y > height) {
            height = p->y;
        }
    }

    for(int i = 0; i <= height; i++) {
        for(int j = 0; j <= width; j++) {
            point_t* p = NULL;

            for(int k = 0; k < array_size; k++) {
                point_t* pi = *(array + k);
                if(pi->x == j && pi->y == i) {
                    p = pi;
                    break;
                }
            }

            printf("%s%c\033[0m", p == NULL ? "" : "\033[31m", p == NULL ? '.' : '#');
        }

        printf("\n");
    }
}

void fold(point_t** array, int array_size, fold_command_t* command) {
    int width = 0;
    int height = 0;

    for(int i = 0; i < array_size; i++) {
        point_t* p = *(array + i);

        if(p->x > width) {
            width = p->x;
        }

        if(p->y > height) {
            height = p->y;
        }
    }

    for(int i = 0; i < array_size; i++) {
        point_t* p = *(array + i);

        if(command->direction == X) {
            if(p->x > command->place) {
                p->x = command->place - (p->x - command->place);
            }
        } else {
            if(p->y > command->place) {
                p->y = command->place - (p->y - command->place);
            }
        }
    }
}

int count_visible(point_t** array, int array_size) {
    int width = 0;
    int height = 0;

    for(int i = 0; i < array_size; i++) {
        point_t* p = *(array + i);

        if(p->x > width) {
            width = p->x;
        }

        if(p->y > height) {
            height = p->y;
        }
    }

    int count = 0;

    for(int i = 0; i <= height; i++) {
        for(int j = 0; j <= width; j++) {
            for(int k = 0; k < array_size; k++) {
                point_t* pi = *(array + k);
                if(pi->x == j && pi->y == i) {
                    count++;
                    break;
                }
            }
        }
    }

    return count;
}

DAY_FUNC(day_13) {
    #ifdef NDEBUG
        #define FILE_NAME "in_13.txt";
    #else
        #define FILE_NAME "in_13_test.txt";
    #endif

    DAY_BOILERPLATE("Day 13", 13, FILE_NAME)

    int point_array_size = 3;
    int point_array_use = 0;
    point_t** point_array = malloc(sizeof(point_t*) * point_array_size);
    memset(point_array, 0, sizeof(point_t*) * point_array_size);

    int command_array_size = 3;
    int command_array_use = 0;
    fold_command_t** command_array = malloc(sizeof(fold_command_t*) * command_array_size);
    memset(command_array, 0, sizeof(fold_command_t*) * command_array_size);

    for(unsigned long i = 0; i < file_lines_count; i++) {
        char* line = *(file_lines + i);

        if(count_character(line, ',') == 1) {
            int x = 0;
            int y = 0;
            sscanf(line, "%d,%d", &x, &y);

            point_t* point = malloc(sizeof(point_t));
            point->x = x;
            point->y = y;

            if(point_array_use + 1 >= point_array_size) {
                point_array_size *= 2;
                point_array = realloc(point_array, sizeof(point_t*) * point_array_size);
            }

            *(point_array + point_array_use++) = point;
        } else if(strlen(line) > 2) {
            char* line_data = line + 11;

            char* dir_token = strdup(strtok(line_data, "="));
            char* num_token = strdup(strtok(NULL, "-"));

            *(num_token + strcspn(num_token, " \r\n")) = '\0';

            direction_t direction = X;
            if(strcmp(dir_token, "y") == 0) {
                direction = Y;
            }

            free(dir_token);

            int along = (int) strtol(num_token, NULL, 10);
            free(num_token);

            fold_command_t* fold_command = malloc(sizeof(fold_command_t));
            fold_command->direction = direction;
            fold_command->place = along;

            if(command_array_use + 1 >= command_array_size) {
                command_array_size *= 2;
                command_array = realloc(command_array, sizeof(fold_command_t*) * command_array_size);
            }

            *(command_array + command_array_use++) = fold_command;
        }
    }

    int visible_after_first = 0;
    for(int i = 0; i < command_array_use; i++) {
        fold_command_t* command = *(command_array + i);
        fold(point_array, point_array_use, command);

        if(i == 0) {
            visible_after_first = count_visible(point_array, point_array_use);
        }
    }

    LOG("Day 13", "Visible after first fold: " UNDERLINE("%d"), visible_after_first);

    LOG("Day 13", "==========");
    print_point_array(point_array, point_array_use);
    LOG("Day 13", "==========");

    for(int i = 0; i < command_array_use; i++) {
        free(*(command_array + i));
    }

    free(command_array);

    for(int i = 0; i < point_array_use; i++) {
        free(*(point_array + i));
    }

    free(point_array);

    DAY_FUNC_END
}
