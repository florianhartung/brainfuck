#include <malloc.h>
#include <string.h>
#include <windows.h>
#include <stdio.h>

#include "brainfuck_interpreter.h"

InterpreterState* create_interpreter(const size_t initial_memory_size,
                                     char* instructions,
                                     const size_t instruction_size) {
    InterpreterState* interpreter = malloc(sizeof(InterpreterState));
    if (interpreter == NULL) {
        fprintf(stderr, "[ERROR] Could not allocate memory for interpreter\n");
        exit(EXIT_FAILURE);
    }

    interpreter->instructions = instructions;
    interpreter->instruction_size = instruction_size;
    interpreter->instruction_pointer = interpreter->instructions;

    interpreter->memory = calloc(initial_memory_size, sizeof(uint8_t));
    interpreter->memory_size = initial_memory_size;
    interpreter->memory_pointer = interpreter->memory;

    return interpreter;
}


static void traverse_code(InterpreterState* interpreter) {
    int loopDepth = 0;
    int direction = *interpreter->instruction_pointer == '[' ? 1 : -1;
    for (; *interpreter->instruction_pointer != '\0'; interpreter->instruction_pointer += direction) {
        if (*interpreter->instruction_pointer == '[')
            loopDepth++;
        else if (*interpreter->instruction_pointer == ']')
            loopDepth--;

        if (loopDepth == 0) break;
    }
}

static void expand_memory(InterpreterState* interpreter, int move) {
    uint8_t* buffer = realloc(interpreter->memory, interpreter->memory_size * 2 * sizeof(uint8_t));
    if (buffer == NULL) {
        fprintf(stderr, "[ERROR] Could not use reallocate memory for an expansion of the brainfuck memory\n");
        exit(EXIT_FAILURE);
    }
    interpreter->memory = buffer;
    if (move) {
        memmove(interpreter->memory + interpreter->memory_size, interpreter->memory, interpreter->memory_size);
        interpreter->memory_pointer += interpreter->memory_size;
    }
    interpreter->memory_size *= 2;
}

int interpret_code(InterpreterState* interpreter) {
    for (; *interpreter->instruction_pointer != '\0'; interpreter->instruction_pointer++) {
        switch (*interpreter->instruction_pointer) {
            case '+':
                ++*interpreter->memory_pointer;
                break;
            case '-':
                --*interpreter->memory_pointer;
                break;
            case '<':
                if (--interpreter->memory_pointer < interpreter->memory) {
                    expand_memory(interpreter, 1);
                }
                break;
            case '>':
                if (++interpreter->memory_pointer == interpreter->memory + interpreter->memory_size) {
                    expand_memory(interpreter, 0);
                }
                break;
            case '[':
                if (!*interpreter->memory_pointer) {
                    traverse_code(interpreter);
                }
                break;
            case ']':
                if (*interpreter->memory_pointer) {
                    traverse_code(interpreter);
                }
                break;
            case ',':
                SetConsoleTitle("Waiting for user input...");
                *interpreter->memory_pointer = getchar();
                printf("\b \b");
                SetConsoleTitle("");
                break;
            case '.':
                putchar(*interpreter->memory_pointer);
                break;
        }
    }
    return EXIT_SUCCESS;
}