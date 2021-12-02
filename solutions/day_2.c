#include "days.h"

typedef enum direction_t {
    FORWARD, UP, DOWN
} direction_t;

typedef struct command_t {
    direction_t direction;
    int amount;

    struct command_t* next;
} command_t;

command_t* get_commands_head(command_t* list) {
    while(list->next != NULL) {
        list = list->next;
    }

    return list;
}

command_t* create_command(direction_t direction, int amount) {
    command_t* command = malloc(sizeof(command_t));
    memset(command, 0, sizeof(command_t));

    command->direction = direction;
    command->amount = amount;

    return command;
}

void free_list(command_t* list) {
    if(list == NULL) {
        return;
    }

    do {
        command_t* temp = list->next;
        free(list);
        list = temp;
    } while (list != NULL);
}

direction_t convert_direction(char* dir_string) {
    if(strcmp(dir_string, "up") == 0) {
        return UP;
    }

    if(strcmp(dir_string, "down") == 0) {
        return DOWN;
    }

    if(strcmp(dir_string, "forward") == 0) {
        return FORWARD;
    }

    ERR("Day 2", "Failed to decode direction!");
    return 0;
}

DAY_FUNC(day_2) {
    char* file_name = "in_2.txt";
    if(execution_info->parameter_count > 1) {
        file_name = execution_info->program_parameters[1];
    }

    FILE* in_file = fopen(file_name, "r");
    if(in_file == NULL) {
        ERR("Day 2", "Failed to open file!");
        return;
    }

    unsigned long line_size = sizeof(char) * 64;
    char* line_read = malloc(line_size);

    command_t* list = create_command(UP, 0);

    while(getline(&line_read, &line_size, in_file) != EOF) {
        char* space_position = strchr(line_read, ' ');

        unsigned long direction_length = (space_position - line_read) + 1;
        char* direction_str = malloc(sizeof(char) * direction_length);
        memcpy(direction_str, line_read, direction_length - 1);
        direction_str[direction_length - 1] = '\0';

        char* movement_remainder;
        int movement_amount = (int) strtol(space_position + 1, &movement_remainder, 10);

        if(strlen(movement_remainder) != 1) {
            ERR("Day 2", "Failed to parse number!");
            continue;
        }

        command_t* new_command = create_command(UP, movement_amount);
        new_command->direction = convert_direction(direction_str);
        new_command->amount = movement_amount;
        get_commands_head(list)->next = new_command;

        free(direction_str);
    }

    free(line_read);

    int horizontal_position = 0;
    int depth = 0;

    command_t* _list = list;
    while(_list != NULL) {
        int* val_to_modify;
        int multiplier = 1;

        switch (_list->direction) {
            case FORWARD:
                val_to_modify = &horizontal_position;
                break;

            case UP:
                multiplier = -1;
            case DOWN:
                val_to_modify = &depth;
                break;
        }

        (*val_to_modify) += (_list->amount * multiplier);

        _list = _list->next;
    }

    LOG_("Day 2", "Final position: " UNDERLINE("%d"), horizontal_position * depth);

    horizontal_position = 0;
    depth = 0;
    int aim = 0;

    _list = list;
    while(_list != NULL) {
        switch (_list->direction) {
            case FORWARD:
                horizontal_position += _list->amount;
                depth += aim * _list->amount;
                break;
            case UP:
                aim -= _list->amount;
                break;
            case DOWN:
                aim += _list->amount;
                break;
        }

        _list = _list->next;
    }

    LOG_("Day 2", "Final position (aim): " UNDERLINE("%d"), horizontal_position * depth);

    free_list(list);
}