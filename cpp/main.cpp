#include <vector>
#include <string>

#include "BrainfuckInterpreter.h"
#include "Util.h"

BrainfuckInterpreter ParseCommandLineArgs(int argc, char** argv) {
    BrainfuckInterpreter interpreter;
    std::vector<std::string> commandLineArgs(argv + 1, argv + argc);
    for (int i = 0; i < commandLineArgs.size() - 1; i += 2) {
        auto flag = commandLineArgs[i];
        auto val = commandLineArgs[i + 1];
        if (flag == "-bf") {
            interpreter.Read(val);
        }
        else if (flag == "-i") {
            interpreter.SetCommandLineInputs(val);
        }
    }
    return interpreter;
}

int main(int argc, char** argv) {

    //auto interpreter = ParseCommandLineArgs(argc, argv);
    BrainfuckInterpreter interpreter;
    interpreter.Read("mandelbrot.txt");
    interpreter.Run();

    std::cout << "Press any key to continue...";
}