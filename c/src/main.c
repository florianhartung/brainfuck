#include <stdio.h>
#include "brainfuck_interpreter.h"
#include "util.h"

int main() {
    char* instructions = 0;
    size_t instruction_size = 0;
    if(read_file("program.bf", &instructions, &instruction_size) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    InterpreterState* interpreter = create_interpreter(3000, instructions, instruction_size);
    interpret_code(interpreter);
    return 0;
}
