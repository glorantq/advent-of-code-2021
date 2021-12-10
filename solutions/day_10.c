#include "days.h"

typedef struct stack_frame_t {
    char open_bracket;
    struct stack_frame_t* next;
} stack_frame_t;

stack_frame_t* create_stack_frame(char c) {
    stack_frame_t* stack_frame = malloc(sizeof(stack_frame_t));
    memset(stack_frame, 0, sizeof(stack_frame_t));

    stack_frame->open_bracket = c;

    return stack_frame;
}

void free_stack_frame(stack_frame_t* frame) {
    while(frame != NULL) {
        stack_frame_t* next = frame->next;
        free(frame);
        frame = next;
    }
}

stack_frame_t* get_head_frame(stack_frame_t* stack) {
    while(stack->next != NULL) {
        stack = stack->next;
    }

    return stack;
}

void push(stack_frame_t* stack, stack_frame_t* frame) {
    get_head_frame(stack)->next = frame;
}

char pop(stack_frame_t* stack) {
    stack_frame_t* head = get_head_frame(stack);

    while (stack->next != head) {
        stack = stack->next;
    }

    stack->next = NULL;

    char ret = head->open_bracket;
    free_stack_frame(head);

    return ret;
}

int get_stack_size(stack_frame_t* stack) {
    if(stack == NULL) {
        return 0;
    }

    return 1 + get_stack_size(stack->next);
}

void sort_array(unsigned long* array, unsigned long size) {
    for(unsigned long i = 0; i < size; i++) {
        unsigned long val = *(array + i);

        unsigned long min_val = val;
        unsigned long min_idx = i;

        for(unsigned long j = i + 1; j < size; j++) {
            unsigned long mv = *(array + j);
            if(mv < min_val) {
                min_val = mv;
                min_idx = j;
            }
        }

        *(array + i) = min_val;
        *(array + min_idx) = val;
    }
}

// Still fine
#pragma clang diagnostic push
#pragma ide diagnostic ignored "DanglingPointer"
DAY_FUNC(day_10) {
    DAY_BOILERPLATE("Day 10", 10, "in_10.txt")

    int syntax_score = 0;

    for(unsigned long i = 0; i < file_lines_count; i++) {
        stack_frame_t* bracket_stack = create_stack_frame('.');

        char* line = *(file_lines + i);
        unsigned long line_length = strcspn(line, " \r\n");

        char expected = '\0';
        char found;

        for(unsigned long j = 0; j < line_length; j++) {
            char c = *(line + j);

            if(strchr("([{<", c) != NULL) {
                push(bracket_stack, create_stack_frame(c));
                continue;
            }

            char open_bracket = pop(bracket_stack);
            if(open_bracket == '[' && c != ']') {
                expected = ']';
            }

            if(open_bracket == '(' && c != ')') {
                expected = ')';
            }

            if(open_bracket == '{' && c != '}') {
                expected = '}';
            }

            if(open_bracket == '<' && c != '>') {
                expected = '>';
            }

            if(expected != '\0') {
                found = c;
                break;
            }
        }

        if(expected != '\0') {
            printf("Expected: %c, but found %c instead.\n", expected, found);

            switch (found) {
                case '}':
                    syntax_score += 1197;
                    break;

                case ']':
                    syntax_score += 57;
                    break;

                case ')':
                    syntax_score += 3;
                    break;

                case '>':
                    syntax_score += 25137;
                    break;

                default:
                    break;
            }

            *line = '\0';
        }

        free_stack_frame(bracket_stack);
    }

    LOG("Day 10", "Syntax error score: " UNDERLINE("%d"), syntax_score);

    unsigned long line_scores_used = 0;
    unsigned long* line_scores = malloc(sizeof(unsigned long) * file_lines_count);
    memset(line_scores, 0, sizeof(unsigned long) * file_lines_count);

    for(unsigned long i = 0; i < file_lines_count; i++) {
        char* line = *(file_lines + i);
        unsigned long line_length = strcspn(line, " \r\n");
        if(line_length == 0) {
            continue;
        }

        stack_frame_t* bracket_stack = create_stack_frame('.');

        for(unsigned long j = 0; j < line_length; j++) {
            char c = *(line + j);

            if(strchr("([{<", c)) {
                push(bracket_stack, create_stack_frame(c));
            }

            if(strchr(")]}>", c)) {
                pop(bracket_stack);
            }
        }

        unsigned long closing_seq_used = 0;
        char* closing_seq = malloc(sizeof(char) * line_length);
        memset(closing_seq, 0, sizeof(char) * line_length);

        int stack_size = get_stack_size(bracket_stack);
        for(int j = 0; j < stack_size - 1; j++) {
            char c = pop(bracket_stack);

            char cl = '\0';
            switch (c) {
                case '(':
                    cl = ')';
                    break;

                case '[':
                    cl = ']';
                    break;

                case '{':
                    cl = '}';
                    break;

                case '<':
                    cl = '>';
                    break;

                default:
                    break;
            }

            if(cl == '\0') {
                continue;
            }

            *(closing_seq + (closing_seq_used++)) = cl;
        }

        unsigned long line_score = 0;
        for(int j = 0; j < closing_seq_used; j++) {
            char c = *(closing_seq + j);
            line_score *= 5;

            switch (c) {
                case ')':
                    line_score += 1;
                    break;

                case ']':
                    line_score += 2;
                    break;

                case '}':
                    line_score += 3;
                    break;

                case '>':
                    line_score += 4;
                    break;

                default:
                    break;
            }
        }

        *(line_scores + (line_scores_used++)) = line_score;

        free(closing_seq);
        free_stack_frame(bracket_stack);
    }

    sort_array(line_scores, line_scores_used);
    unsigned long file_score = *(line_scores + (line_scores_used / 2));

    LOG("Day 10", "Completion score: " UNDERLINE("%lu"), file_score);

    free(line_scores);
    DAY_FUNC_END
}
#pragma clang diagnostic pop
