#include <malloc.h>
#include <string.h>
#include <windows.h>

#include "brainfuck_interpreter.h"
#include "util.h"

static void read_into_interpreter(InterpreterState* interpreter, const char* filename) {
    read_file(filename, &interpreter->instructions, &interpreter->instruction_size);
    interpreter->memory_size = DEFAULT_TAPE_SIZE;
    interpreter->memory = calloc(DEFAULT_TAPE_SIZE, sizeof(uint8_t));
    interpreter->cell = interpreter->memory;
    interpreter->curr_instruction = interpreter->instructions;
}

InterpreterState* create_interpreter(const char* filename) {
    InterpreterState* interpreter = malloc(sizeof(InterpreterState));
    if(interpreter == NULL) {
        printf("IN create_interpreter Interpreter is Null\n");
    }
    read_into_interpreter(interpreter, filename);
    return interpreter;
}

void reset_interpreter(InterpreterState* interpreter, const char* filename) {
    free(interpreter->memory);
    free(interpreter->instructions);
    interpreter->memory_size = DEFAULT_TAPE_SIZE;
    interpreter->instruction_size = 0;
    read_into_interpreter(interpreter, filename);
}

static void traverse_code(InterpreterState* interpreter) {
    int loopDepth = 0;
    int direction = *interpreter->curr_instruction == '[' ? 1 : -1;
    for(;*interpreter->curr_instruction != '\0'; interpreter->curr_instruction += direction) {
        if(*interpreter->curr_instruction == '[')
            loopDepth++;
        else if(*interpreter->curr_instruction == ']')
            loopDepth--;

        if(loopDepth == 0) break;
    }
}

static void expand_memory(InterpreterState* interpreter, int move) {
    uint8_t* buffer = realloc(interpreter->memory, interpreter->memory_size * 2 *  sizeof(uint8_t));
    if(buffer == NULL) {
        printf("[ERROR] Could not use realloc\n");
        exit(EXIT_FAILURE);
    }
    interpreter->memory = buffer;
    if(move) {
        memmove(interpreter->memory + interpreter->memory_size, interpreter->memory, interpreter->memory_size);
        interpreter->cell += interpreter->memory_size;
    }
    interpreter->memory_size *= 2;
}

int interpret_code(InterpreterState* interpreter) {
    for(; *interpreter->curr_instruction != '\0'; interpreter->curr_instruction++) {
        char token = *interpreter->curr_instruction;
        switch (token) {
        case '+':
            ++*interpreter->cell;
            break;
        case '-':
            --*interpreter->cell;
            break;
        case '<':
            if (--interpreter->cell < interpreter->memory) {
                expand_memory(interpreter, 1);
            }
            break;
        case '>':
            if (++interpreter->cell == interpreter->memory + interpreter->memory_size) {
                expand_memory(interpreter, 0);
            }
            break;
        case '[':
            if (!*interpreter->cell) {
                traverse_code(interpreter);
            }
            break;
        case ']':
            if (*interpreter->cell) {
                traverse_code(interpreter);
            }
            break;
        case ',':
            SetConsoleTitle("Waiting for user input...");
            *interpreter->cell = getchar();
            printf("\b \b");
            SetConsoleTitle("");
            break;
        case '.':
            putchar(*interpreter->cell);
            break;
        }
    }
    return EXIT_SUCCESS;
}