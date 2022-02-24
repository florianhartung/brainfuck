#include "util.h"

#include <stdio.h>
#include <stdlib.h>

int read_file(const char* path, char** content, size_t* contentSize) {
    FILE* file = fopen(path, "r");

    if(file == NULL) {
        fprintf(stderr,"[ERROR] Failed to open file %s", path);
        return EXIT_FAILURE;
    }

    fseek(file, 0, SEEK_END);
    *contentSize = ftell(file);
    rewind(file);

    if(*contentSize == -1ull) {
        fprintf(stderr, "[ERROR] Failed to get file size of file %s", path);
        return EXIT_FAILURE;
    }

    free(*content);
    *content = calloc(*contentSize + 1, sizeof(char));
    fread(*content, sizeof(char), *contentSize, file);

    fclose(file);

    return EXIT_SUCCESS;
}