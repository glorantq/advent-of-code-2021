#include "days.h"
#include "../util/math.h"

// It's fiiiine
#pragma clang diagnostic push
#pragma ide diagnostic ignored "DanglingPointer"
bool are_characters_same(char* first, char* second) {
    if (first == NULL || second == NULL) {
        return false;
    }

    unsigned long first_len = strlen(first);
    unsigned long second_len = strlen(second);

    if (first_len != second_len) {
        return false;
    }

    for (unsigned long i = 0; i < first_len; i++) {
        char c = first[i];

        bool found = false;
        for (unsigned long j = 0; j < second_len; j++) {
            char c1 = second[j];
            if (c == c1) {
                found = true;
                break;
            }
        }

        if (!found) {
            return false;
        }
    }

    return true;
}

char* strsub(char* first, char* second) {
    unsigned long first_length = strlen(first);
    unsigned long second_length = strlen(second);
    unsigned long new_length = (MAX(first_length, second_length)) + 1;

    char* new_string = malloc(sizeof(char) * new_length);
    memset(new_string, 0, sizeof(char) * new_length);
    unsigned long new_used = 0;

    for (unsigned long i = 0; i < first_length; i++) {
        char c = *(first + i);

        bool contains = false;
        for (unsigned long j = 0; j < second_length; j++) {
            char c1 = *(second + j);
            if (c == c1) {
                contains = true;
            }
        }

        if (!contains) {
            *(new_string + (new_used++)) = c;
        }
    }

    return new_string;
}

DAY_FUNC(day_8) {
    DAY_BOILERPLATE("Day 8", 8, "in_8.txt")

    int simple_digit_count = 0;

    for (unsigned long i = 0; i < file_lines_count; i++) {
        char* line = strdup(*(file_lines + i));
        char* token = strtok(strchr(line, '|') + 2, " ");

        while (token != NULL) {
            unsigned long seq_length = strcspn(token, "\r\n ");
            *(token + seq_length) = '\0';

            if (seq_length != 5 && seq_length != 6 && seq_length > 0) {
                simple_digit_count++;
            }

            token = strtok(NULL, " ");
        }

        free(line);
    }

    LOG("Day 8", "Simple digit count: " UNDERLINE("%d"), simple_digit_count);

    int total_sum = 0;
    for(unsigned long i = 0; i < file_lines_count; i++) {
        char* line = strdup(*(file_lines + i));
        char* line_2 = strdup(*(file_lines + i));
        *(line + strcspn(line, "|")) = '\0';

        char** digits_mapping = malloc(sizeof(char*) * 10);
        memset(digits_mapping, 0, sizeof(char*) * 10);

        int decode_array_used = 0;
        char** decode_token_array = malloc(sizeof(char*) * 10);
        memset(decode_token_array, 0, sizeof(char*) * 10);

        char* token = strtok(line, " ");
        while(token != NULL) {
            *(token + strcspn(token, " \r\n")) = '\0';

            *(decode_token_array + (decode_array_used++)) = strdup(token);

            token = strtok(NULL, " ");
        } // First loop over the random data (before |), storing the tokens for processing

        for(int k = 0; k < 10; k++) {
            char* to_decode = *(decode_token_array + k);
            unsigned long decode_length = strlen(to_decode);

            if(decode_length == 5 || decode_length == 6 || decode_length < 0) {
                continue;
            }

            // 1, 4, 7, 8
            int num_idx;
            switch (decode_length) {
                case 2:
                    num_idx = 1;
                    break;

                case 4:
                    num_idx = 4;
                    break;

                case 3:
                    num_idx = 7;
                    break;

                case 7:
                    num_idx = 8;
                    break;

                default:
                    continue;
            }

            *(digits_mapping + num_idx) = strdup(to_decode);
        } // Find all simple numbers and store them

        for(int l = 0; l < 10; l++) {
            char* to_decode = *(decode_token_array + l);
            unsigned long decode_length = strlen(to_decode);

            char* remaining = strsub(*(digits_mapping + 8), to_decode);

            if(decode_length == 6) { // 6: 0, 6, 9
                char* sub_from_one_r = strsub(*(digits_mapping + 1), remaining);
                char* sub_from_one = strsub(*(digits_mapping + 1), to_decode);
                char* sub_from_four = strsub(*(digits_mapping + 4), to_decode);
                char* sub_one = strsub(to_decode, *(digits_mapping + 1));
                if(strlen(sub_from_one_r) == 1) {
                    *(digits_mapping + 6) = strdup(to_decode);
                }

                if(strlen(sub_from_one) == 0 && strlen(sub_from_four) == 1) {
                    *(digits_mapping + 0) = strdup(to_decode);
                } else if(strlen(sub_one) == 4) {
                    *(digits_mapping + 9) = strdup(to_decode);
                }

                free(sub_from_one_r);
                free(sub_from_four);
                free(sub_from_one);
                free(sub_one);
            } else if(decode_length == 5) { // 5: 2, 3, 5
                char* sub_four = strsub(to_decode, *(digits_mapping + 4));
                if(strlen(sub_four) == 3) {
                    *(digits_mapping + 2) = strdup(to_decode);
                } else {
                    char* sub_one = strsub(to_decode, *(digits_mapping + 1));
                    if(strlen(sub_one) == 3) {
                        *(digits_mapping + 3) = strdup(to_decode);
                    } else if(strlen(sub_one) == 4) {
                        *(digits_mapping + 5) = strdup(to_decode);
                    }

                    free(sub_one);
                }

                free(sub_four);
            }

            free(remaining);
        } // Find more complex numbers

        int num_place = 1000;
        int line_value = 0;
        char* remainder_line = line_2 + strcspn(line_2, "|") + 2;
        char* token_2 = strtok(remainder_line, " ");
        while(token_2 != NULL) {
            *(token_2 + strcspn(token_2, " \r\n")) = '\0';

            for(int m = 0; m < 10; m++) {
                if(are_characters_same(token_2, *(digits_mapping + m))) {
                    line_value += m * num_place;
                    num_place /= 10;
                }
            }

            token_2 = strtok(NULL, " ");
        }

        total_sum += line_value;

        for(int j = 0; j < 10; j++) {
            if(*(digits_mapping + j) == NULL) {
                WARN("Day 8", "Failed to determine %d!", j);
            } else {
                free(*(digits_mapping + j));
            }

            free(*(decode_token_array + j));
        }
        free(decode_token_array);
        free(digits_mapping);
        free(line);
        free(line_2);
    } // File lines

    LOG("Day 8", "Line sums: " UNDERLINE("%d"), total_sum);

    DAY_FUNC_END
}

#pragma clang diagnostic pop
