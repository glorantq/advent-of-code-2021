#include "days.h"

typedef struct bingo_value_t {
    int number;
    bool marked;
} bingo_value_t;

typedef bingo_value_t*** bingo_board_t;

int count_character(char* string, char c) {
    unsigned long string_length = strlen(string);
    int counter = 0;

    for(unsigned long i = 0; i < string_length; i++) {
        if(string[i] == c) {
            counter++;
        }
    }

    return counter;
}

bingo_value_t* create_value(int value) {
    bingo_value_t* bingo_value = malloc(sizeof(bingo_value_t));
    memset(bingo_value, 0, sizeof(bingo_value_t));

    bingo_value->number = value;

    return bingo_value;
}

bingo_board_t create_board() {
    bingo_value_t*** row_array = malloc(sizeof(bingo_value_t**) * 5);
    memset(row_array, 0, sizeof(bingo_value_t**) * 5);

    for(int i = 0; i < 5; i++) {
        bingo_value_t** row = malloc(sizeof(bingo_value_t*) * 5);
        memset(row, 0, sizeof(bingo_value_t*) * 5);

        for(int j = 0; j < 5; j++) {
            bingo_value_t* val = create_value(0);
            *(row + j) = val;
        }

        *(row_array + i) = row;
    }

    return row_array;
}

void free_board(bingo_board_t board) {
    for(int i = 0; i < 5; i++) {
        bingo_value_t** row = *(board + i);

        for(int j = 0; j < 5; j++) {
            bingo_value_t* val = *(row + j);
            free(val);
        }

        free(row);
    }

    free(board);
}

void debug_print_board(bingo_board_t board) {
    for(int i = 0; i < 5; i++) {
        bingo_value_t** row = *(board + i);

        for(int j = 0; j < 5; j++) {
            bingo_value_t* val = *(row + j);
            if(val->marked) {
                printf("\033[31;1m");
            }

            printf("%2d\033[0m ", val->number);
        }

        printf("\n");
    }
}

bingo_board_t parse_board(char** file_lines, int starting_line_idx) {
    bingo_board_t parsed_board = create_board();

    for(int i = starting_line_idx; i < starting_line_idx + 5; i++) {
        int board_row = i - starting_line_idx;
        char* board_line = *(file_lines + i);

        for(int j = 0; j < 5; j++) {
            int num = -1;
            sscanf(board_line + j * 3, "%d", &num);

            parsed_board[board_row][j]->number = num;
        }
    }

    return parsed_board;
}

bool check_winner(bingo_board_t board) {
    bool has_row_marked = false;
    bool has_col_marked = false;

    for(int i = 0; i < 5; i++) {
        bool _row_marked = true;
        bingo_value_t** row = *(board + i);

        for(int j = 0; j < 5; j++) {
            bingo_value_t* value = *(row + j);
            _row_marked = _row_marked && value->marked;
        }

        has_row_marked = has_row_marked || _row_marked;
    }

    for(int i = 0; i < 5; i++) {
        bool _col_marked = true;
        for(int j = 0; j < 5; j++) {
            bingo_value_t* value = *(*(board + j) + i);
            _col_marked = _col_marked && value->marked;
        }

        has_col_marked = has_col_marked || _col_marked;
    }

    return has_row_marked || has_col_marked;
}

void mark_number(bingo_board_t board, int number) {
    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 5; j++) {
            bingo_value_t* value = *(*(board + i) + j);
            value->marked = value->marked || value->number == number;
        }
    }
}

int calculate_sum_of_unmarked(bingo_board_t board) {
    int sum = 0;

    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 5; j++) {
            bingo_value_t* value = *(*(board + i) + j);

            if(!value->marked) {
                sum += value->number;
            }
        }
    }

    return sum;
}

DAY_FUNC(day_4) {
    DAY_BOILERPLATE("Day 4", 4, "in_4.txt")

    int drawn_numbers_count = count_character(*file_lines, ',') + 1;
    int* drawn_numbers = malloc(sizeof(int) * drawn_numbers_count);
    int _drawn_array_usage = 0;

    char* token = strtok(*file_lines, ",");
    while(token != NULL) {
        char* token_remainder;
        int parsed_number = (int) strtol(token, &token_remainder, 10);
        *(drawn_numbers + (_drawn_array_usage++)) = parsed_number;

        token = strtok(NULL, ",");
    }

    DEBUG("Day 4", "Read %d drawn numbers...", _drawn_array_usage);

    int board_count = (int) ((file_lines_count - 1uL) / 6uL);
    DEBUG("Day 4", "Need to parse %d boards...", board_count);

    bingo_board_t* board_array = malloc(sizeof(bingo_board_t) * board_count);
    for(int i = 0; i < board_count; i++) {
        int start_line_idx = 2 + (i * 6);
        *(board_array + i) = parse_board(file_lines, start_line_idx);
    }

    int winner_idx = -1;
    int loser_idx = -1;
    int num_when_won = 0;
    int num_when_lost = 0;
    int winner_board_counts = 0;

    for(int i = 0; i < _drawn_array_usage; i++) {
        int number_drawn = *(drawn_numbers + i);

        for(int j = 0; j < board_count; j++) {
            bingo_board_t board = *(board_array + j);
            if(!check_winner(board)) {
                mark_number(board, number_drawn);

                if (check_winner(board)) {
                    if (winner_board_counts == board_count - 1) {
                        loser_idx = j;
                        num_when_lost = number_drawn;

                        DEBUG("Day 4", "Last winner %d %d", j, num_when_lost);
                    }

                    if (winner_idx == -1) {
                        winner_idx = j;
                        num_when_won = number_drawn;

                        DEBUG("Day 4", "First winner %d %d", j, num_when_won);
                    }

                    winner_board_counts++;
                }
            }
        }
    }

    if(winner_idx >= 0) {
        bingo_board_t winner_board = *(board_array + winner_idx);
        debug_print_board(winner_board);
        LOG("Day 4", "Board %d won with a score of: " UNDERLINE("%d"), winner_idx + 1, calculate_sum_of_unmarked(winner_board) * num_when_won);
    } else {
        WARN("Day 4", "No winner boards?");
    }

    if(loser_idx > 0) {
        bingo_board_t loser_board = *(board_array + loser_idx);
        debug_print_board(loser_board);
        LOG("Day 4", "Board %d lost with a score of: " UNDERLINE("%d"), loser_idx + 1, calculate_sum_of_unmarked(loser_board) * num_when_lost);
    } else {
        WARN("Day 4", "No loser board?");
    }

    for(int i = 0; i < board_count; i++) {
        free_board(*(board_array + i));
    }

    free(board_array);
    free(drawn_numbers);
    DAY_FUNC_END
}
