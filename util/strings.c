#include "strings.h"
#include <string.h>

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
