#ifndef BRAINFUCK_BRAINFUCK_INTERPRETER_H
#define BRAINFUCK_BRAINFUCK_INTERPRETER_H

#include <stdint.h>

typedef struct {
    size_t memory_size;
    size_t instruction_size;
    uint8_t* memory;
    uint8_t* memory_pointer;
    char* instructions;
    char* instruction_pointer;
} InterpreterState;

InterpreterState* create_interpreter(size_t initial_memory_size, char* instructions, size_t instruction_size);

int interpret_code(InterpreterState* interpreter);

#endif //BRAINFUCK_BRAINFUCK_INTERPRETER_H
