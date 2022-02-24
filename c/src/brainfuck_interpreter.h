#ifndef BRAINFUCK_BRAINFUCK_INTERPRETER_H
#define BRAINFUCK_BRAINFUCK_INTERPRETER_H

#include <stdint.h>

#define DEFAULT_TAPE_SIZE 30000

typedef struct {;
    size_t memory_size;
    size_t instruction_size;
    uint8_t* memory;
    uint8_t* cell;
    char* instructions;
    char* curr_instruction;
} InterpreterState;

InterpreterState* create_interpreter(const char* filename);

void reset_interpreter(InterpreterState* interpreter, const char* filename);

int interpret_code(InterpreterState* interpreter);

#endif //BRAINFUCK_BRAINFUCK_INTERPRETER_H
