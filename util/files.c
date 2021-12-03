#include <string.h>

#include "files.h"
#include "../log.h"

char** read_file_lines(FILE* file, unsigned long* out_line_count) {
    if(file == NULL) {
        ERR("Files", "file == NULL!");
        return NULL;
    }

    unsigned long line_array_used = 0;
    unsigned long line_array_size = sizeof(char*) * 8;
    char** line_array = malloc(line_array_size);

    unsigned long read_line_size = sizeof(char) * 64;
    char* read_line = malloc(read_line_size);

    while(getline(&read_line, &read_line_size, file) != EOF) {
        char* copied = strdup(read_line);

        if(line_array_used + 1 >= line_array_size / sizeof(char*)) {
            line_array_size *= 2;
            char** temp = realloc(line_array, line_array_size);

            if(temp == NULL) {
                ERR("Files", "Failed to re-allocate line array!");

                free(copied);
                free(read_line);

                return NULL;
            }

            line_array = temp;
        }

        *(line_array + (line_array_used++)) = copied;
    }

    free(read_line);

    *out_line_count = line_array_used;
    return line_array;
}

void free_file_lines(char** line_array, unsigned long line_count) {
    for(unsigned long i = 0; i < line_count; i++) {
        free(*(line_array + i));
    }

    free(line_array);
}
