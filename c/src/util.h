#ifndef BRAINFUCK_UTIL_H
#define BRAINFUCK_UTIL_H

#include <stdio.h>
#include <stdlib.h>

static inline int read_file(const char* path, char** content, size_t* contentSize) {
    FILE* file = fopen(path, "r");

    if(file == NULL) {
        printf("[ERROR] Failed to open file");
        return EXIT_FAILURE;
    }

    fseek(file, 0, SEEK_END);
    *contentSize = ftell(file);
    rewind(file);

    if(*contentSize == -1ull) {
        printf("[ERROR] Failed to extract file size");
        return EXIT_FAILURE;
    }

    free(*content);
    *content = calloc(*contentSize + 1, sizeof(char));
    fread(*content, sizeof(char), *contentSize, file);

    fclose(file);

    return EXIT_SUCCESS;
}

inline void clear_console() {
#if defined(_WIN32)
    system("cls");
#endif
#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
    system("clear");
#endif
}

#endif //BRAINFUCK_UTIL_H
