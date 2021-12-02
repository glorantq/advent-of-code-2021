#ifndef ADVENT_OF_CODE_FILES_H
#define ADVENT_OF_CODE_FILES_H

#include <stdio.h>
#include <stdlib.h>

char** read_file_lines(FILE* file, unsigned long* out_line_count);
void free_file_lines(char** line_array, unsigned long line_count);

#endif