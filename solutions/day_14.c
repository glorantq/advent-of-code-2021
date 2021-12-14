#include "days.h"
#include "../util/math.h"
#include "limits.h"

#define NUM_LETTERS ('Z' - 'A' + 1)
#define ARRAY_SIZE NUM_LETTERS * NUM_LETTERS

#define TO_INT(c) ((c) - 'A')
#define TO_IDX(f, s) ((f) * NUM_LETTERS + (s))
#define GET_FIRST(idx) ((idx) / NUM_LETTERS)
#define GET_SECOND(idx) ((idx) % NUM_LETTERS)

void perform_step(const char* insertion_mapping, unsigned long* pair_counts) {
    unsigned long work[ARRAY_SIZE] = { 0 };

    for(int i = 0; i < ARRAY_SIZE; i++) {
        char replacement = *(insertion_mapping + i);
        if(replacement == '\0') {
            continue;
        }

        int first_pair_idx = TO_IDX(GET_FIRST(i), TO_INT(replacement));
        int second_pair_idx = TO_IDX(TO_INT(replacement), GET_SECOND(i));

        (*(work + first_pair_idx)) += (*(pair_counts + i));
        (*(work + second_pair_idx)) += (*(pair_counts + i));
    }

    for(int i = 0; i < ARRAY_SIZE; i++) {
        *(pair_counts + i) = *(work + i);
    }
}

unsigned long get_answer(const unsigned long* pair_count, char template_first, char template_second) {
    unsigned long letter_counts[NUM_LETTERS] = { 0 };

    for(int i = 0; i < ARRAY_SIZE; i++) {
        int first_idx = GET_FIRST(i);
        int second_idx = GET_SECOND(i);

        (*(letter_counts + first_idx)) += *(pair_count + i);
        (*(letter_counts + second_idx)) += *(pair_count + i);
    }

    (*(letter_counts + TO_INT(template_first)))++;
    (*(letter_counts + TO_INT(template_second)))++;

    unsigned long min = ULONG_MAX;
    unsigned long max = 0;

    for(int i = 0; i < NUM_LETTERS; i++) {
        if(*(letter_counts + i) == 0) {
            continue;
        }

        min = MIN(min, *(letter_counts + i));
        max = MAX(max, *(letter_counts + i));
    }

    return (max - min) / 2;
}

void count_initial_pairs(char* polymer_template, unsigned long* pair_counts) {
    unsigned long polymer_length = strlen(polymer_template);

    for(int i = 0; i < polymer_length - 1; i++) {
        char f = *(polymer_template + i);
        char s = *(polymer_template + i + 1);

        int idx = TO_IDX(TO_INT(f), TO_INT(s));
        (*(pair_counts + idx))++;
    }
}

DAY_FUNC(day_14) {
    DAY_BOILERPLATE("Day 14", 14, "in_14.txt")

    char* insertion_mapping = malloc(sizeof(char) * ARRAY_SIZE);
    memset(insertion_mapping, 0, sizeof(char) * ARRAY_SIZE);

    unsigned long* pair_counts = malloc(sizeof(unsigned long) * ARRAY_SIZE);
    memset(pair_counts, 0, sizeof(unsigned long) * ARRAY_SIZE);

    char* polymer_template = *file_lines;
    unsigned long polymer_length = strcspn(polymer_template, " \r\n");

    char first_letter = *polymer_template;
    char last_letter = *(polymer_template + polymer_length - 1);

    for(unsigned long i = 2; i < file_lines_count; i++) {
        char* line = strdup(*(file_lines + i));

        if(strlen(line) > 2) {
            char* find = strdup(strtok(line, " -> "));
            char insert = *strtok(NULL, " -> ");

            *(insertion_mapping + TO_IDX(TO_INT(*find), TO_INT(*(find + 1)))) = insert;
        }

        free(line);
    }

    count_initial_pairs(polymer_template, pair_counts);

    for(int i = 0; i < 10; i++) {
        perform_step(insertion_mapping, pair_counts);
    }

    LOG("Day 14", "Part 1: " UNDERLINE("%lu"), get_answer(pair_counts, first_letter, last_letter));

    memset(pair_counts, 0, sizeof(unsigned long) * ARRAY_SIZE);
    count_initial_pairs(polymer_template, pair_counts);

    for(int i = 0; i < 40; i++) {
        perform_step(insertion_mapping, pair_counts);
    }

    LOG("Day 14", "Part 2: " UNDERLINE("%lu"), get_answer(pair_counts, first_letter, last_letter));

    free(insertion_mapping);
    free(pair_counts);

    DAY_FUNC_END
}
