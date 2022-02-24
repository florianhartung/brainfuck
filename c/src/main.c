#include <stdio.h>

#include "brainfuck_interpreter.h"

int main() {
    InterpreterState* interpreter = create_interpreter("program.bf");
    interpret_code(interpreter);
    return 0;
}
