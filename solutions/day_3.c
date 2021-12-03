#include "days.h"

int decode_number(char* binary_string) {
    unsigned long string_length = strcspn(binary_string, "\r\n");
    int decoded_number = 0;

    for(unsigned long i = 0; i < string_length; i++) {
        char c = binary_string[i];
        decoded_number |= (c == '1' ? 1 : 0);

        if(i != string_length - 1) {
            decoded_number <<= 1;
        }
    }

    return decoded_number;
}

int most_common_bit(const int numbers[], int array_size, int place, bool* eq) {
    int ones_count = 0;
    int zeros_count = 0;

    for(int i = 0; i < array_size; i++) {
        int bit_in_place = ((*(numbers + i)) >> place) & 1;

        if(bit_in_place == 1) {
            ones_count++;
        } else {
            zeros_count++;
        }
    }

    if(ones_count == zeros_count && eq != NULL) {
        *eq = true;
    }

    if(ones_count > zeros_count) {
        return 1;
    }

    return 0;
}

int* get_numbers_matching_criteria(const int numbers[], int array_length, int criteria, int place, int* out_size) {
    int* numbers_out = malloc(sizeof(int) * array_length);
    if(numbers_out == NULL) {
        return NULL;
    }

    int count = 0;

    for(int i = 0; i < array_length; i++) {
        int bit_in_place = ((*(numbers + i)) >> place) & 1;

        if(bit_in_place == criteria) {
            *(numbers_out + count) = *(numbers + i);
            count++;
        }
    }

    *out_size = count;
    return numbers_out;
}

int find_generator_rating(int numbers[], int array_length, bool invert, int places) {
    bool eq = false;

    int* work = malloc(sizeof(int) * array_length);
    if(work == NULL) {
        return 0;
    }

    memcpy(work, numbers, sizeof(int) * array_length);

    for(int i = 0; i < places; i++) {
        int check_bit = most_common_bit(work, array_length, places - i - 1, &eq);

        if(invert) {
            check_bit = ~check_bit & 1;
        }

        if(eq) {
            check_bit = invert ? 0 : 1;
        }

        int* matching = get_numbers_matching_criteria(work, array_length, check_bit, places - i - 1, &array_length);
        if(matching == NULL) {
            break;
        }

        free(work);
        work = matching;

        if(array_length == 1) {
            int val = *work;
            free(work);

            return val;
        }
    }

    free(work);
    return 0;
}

DAY_FUNC(day_3) {
    OPEN_DAY_INPUT(3, "in_3.txt");
    READ_DAY_INPUT(in_file);

    int* numbers = malloc(sizeof(int) * file_lines_count);
    if(numbers == NULL) {
        ERR("Day 3", "Failed to allocate memory!");

        free_file_lines(file_lines, file_lines_count);
        fclose(in_file);
        return;
    }

    memset(numbers, 0, sizeof(int) * file_lines_count);

    unsigned int most_bits = 0;

    for(unsigned long i = 0; i < file_lines_count; i++) {
        char* line = *(file_lines + i);
        unsigned int num_length = strcspn(line, "\r\n");

        if(num_length > most_bits) {
            most_bits = num_length;
        }

        *(numbers + i) = decode_number(line);
    }

    DEBUG_("Day 3", "Converted %lu binary numbers...", file_lines_count);

    free_file_lines(file_lines, file_lines_count);
    fclose(in_file);

    unsigned int mask = 0;
    for(int i = 0; i < most_bits; i++) {
        mask |= 1 << i;
    }

    unsigned int gamma_rate = 0;

    for(int i = 0; i < most_bits; i++) {
        gamma_rate |= (most_common_bit(numbers, (int) file_lines_count, i, NULL)) << i;
    }

    unsigned int epsilon_rate = (~gamma_rate) & mask;

    LOG_("Day 3", "Power consumption: " UNDERLINE("%d"), gamma_rate * epsilon_rate);

    int oxygen_rating = find_generator_rating(numbers, (int) file_lines_count, false, (int) most_bits);
    int dioxide_rating = find_generator_rating(numbers, (int) file_lines_count, true, (int) most_bits);
    LOG_("Day 3", "Life support rating: " UNDERLINE("%d"), oxygen_rating * dioxide_rating);

    free(numbers);
}