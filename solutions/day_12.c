#include "days.h"
#include "ctype.h"

typedef struct cave_t {
    int id;
    char* name;
    int num_connections;
    struct cave_t** connections;
} cave_t;

cave_t* create_cave(char* name, int id) {
    cave_t* cave = malloc(sizeof(cave_t));
    memset(cave, 0, sizeof(cave_t));

    cave->id = id;
    cave->name = name;
    cave->connections = malloc(sizeof(cave_t*) * 32);

    return cave;
}

// Just the usual
#pragma clang diagnostic push
#pragma ide diagnostic ignored "DanglingPointer"
cave_t* find_or_create_cave(cave_t** cave_array, int* cave_array_used, char* name, int* id) {
    cave_t* found = NULL;

    for(int i = 0; i < *cave_array_used; i++) {
        cave_t* it = *(cave_array + i);
        if(strcmp(it->name, name) == 0) {
            found = it;

            free(name);
            break;
        }
    }

    if(found == NULL) {
        found = create_cave(name, (*id)++);
        *(cave_array + (*cave_array_used)++) = found;
    }

    return found;
}
#pragma clang diagnostic pop

cave_t* create_path(cave_t* start) {
    cave_t* path = malloc(sizeof(cave_t));
    memset(path, 0, sizeof(cave_t));

    path->name = strdup(start->name);
    path->id = start->id;
    path->connections = malloc(sizeof(cave_t*));
    *path->connections = NULL;

    return path;
}

cave_t* copy_path(cave_t* path) {
    cave_t* new_start = create_path(path);

    cave_t* conn = *path->connections;
    if(conn != NULL) {
        cave_t* new_conn = copy_path(conn);
        *new_start->connections = new_conn;
    }

    return new_start;
}

void free_path(cave_t* path) {
    while(path != NULL) {
        cave_t* c = *path->connections;

        free(path->name);
        free(path->connections);
        free(path);

        path = c;
    }
}

void append_to_path(cave_t* cave, cave_t* path) {
    cave_t* extension = create_path(cave);

    while(*path->connections != NULL) {
        path = *path->connections;
    }

    *path->connections = extension;
}

bool is_valid_next_part_1(cave_t* cave, cave_t* path) {
    int num_visited = 0;

    while(path != NULL) {
        if(path->id == cave->id) {
            num_visited++;
        }

        path = *path->connections;
    }

    if(islower(*cave->name)) {
        return num_visited == 0;
    } else {
        return true;
    }
}

bool is_valid_next_part_2(cave_t* cave, cave_t* path) {
    if(strcmp(cave->name, "start") == 0) {
        return false;
    }

    int small_cave_visits[64] = { 0 };
    for(int i = 0; i < 64; i++) {
        *(small_cave_visits + i) = 0;
    }

    while(path != NULL) {
        if(islower(*path->name)) {
            *(small_cave_visits + path->id) += 1;
        }

        path = *path->connections;
    }

    bool has_visited_twice = false;
    for(int i = 0; i < 64; i++) {
        int visit_count = *(small_cave_visits + i);

        if(visit_count > 1) {
            has_visited_twice = true;
            break;
        }
    }

    if(islower(*cave->name)) {
        if(has_visited_twice) {
            return *(small_cave_visits + cave->id) < 1;
        } else {
            return *(small_cave_visits + cave->id) < 2;
        }
    } else {
        return true;
    }
}

void walk_paths(cave_t* cave, cave_t* path, int* out_count, bool (*cave_filter)(cave_t*, cave_t*)) {
    if(strcmp(cave->name, "start") != 0) {
        append_to_path(cave, path);
    }

    if(strcmp(cave->name, "end") == 0) {
#ifndef NDEBUG
        if(*out_count <= 40) {
            while (path != NULL) {
                printf("%s, ", path->name);
                path = *path->connections;
            }
            printf("\n");
        }
#endif

        (*out_count)++;
        return;
    }

    for(int i = 0; i < cave->num_connections; i++) {
        cave_t* connection = *(cave->connections + i);

        if((*cave_filter)(connection, path)) {
            cave_t* path_copy = copy_path(path);
            walk_paths(connection, path_copy, out_count, cave_filter);
            free_path(path_copy);
        }
    }
}

DAY_FUNC(day_12) {
    #ifdef NDEBUG
        #define FILE_NAME "in_12.txt";
    #else
        #define FILE_NAME "in_12_test.txt";
    #endif

    DAY_BOILERPLATE("Day 12", 12, FILE_NAME)

    int cave_list_used = 0;
    cave_t** cave_list = malloc(sizeof(cave_t*) * file_lines_count);
    memset(cave_list, 0, sizeof(cave_t*) * file_lines_count);

    int id = 0;
    for(unsigned long i = 0; i < file_lines_count; i++) {
        char* line = *(file_lines + i);

        char* start_name = strdup(strtok(line, "-"));
        char* end_name = strdup(strtok(NULL, "-"));

        *(end_name + strcspn(end_name, " \r\n")) = '\0';

        cave_t* start_cave = find_or_create_cave(cave_list, &cave_list_used, start_name, &id);
        cave_t* end_cave = find_or_create_cave(cave_list, &cave_list_used, end_name, &id);

        *(start_cave->connections + start_cave->num_connections++) = end_cave;
        *(end_cave->connections + end_cave->num_connections++) = start_cave;
    }

    cave_t* start_cave = NULL;
    for(int i = 0; i < cave_list_used; i++) {
        cave_t* it = *(cave_list + i);
        if(strcmp(it->name, "start") == 0) {
            start_cave = it;
            break;
        }
    }

    int part_1_count = 0;
    cave_t* path = create_path(start_cave);
    walk_paths(start_cave, path, &part_1_count, &is_valid_next_part_1);
    free_path(path);

    LOG("Day 12", "Part 1: " UNDERLINE("%d"), part_1_count);

    int part_2_count = 0;
    path = create_path(start_cave);
    walk_paths(start_cave, path, &part_2_count, &is_valid_next_part_2);
    free_path(path);

    LOG("Day 12", "Part 2: " UNDERLINE("%d"), part_2_count);

    for(int i = 0; i < cave_list_used; i++) {
        cave_t* cave = *(cave_list + i);

        free(cave->name);
        free(cave->connections);
        free(cave);
    }

    free(cave_list);
    DAY_FUNC_END
}
