#include <vector>
#include <string>

#include "BrainfuckInterpreter.h"
#include "Util.h"

std::unique_ptr<BrainfuckInterpreter> ParseCommandLineArgs(int argc, char** argv) {
    auto interpreter = std::make_unique<BrainfuckInterpreter>();
    const std::vector<std::string> commandLineArgs(argv + 1, argv + argc);
    for (int i = 0; i < commandLineArgs.size() - 1; i += 2) {
        std::string flag = commandLineArgs[i];
        std::string val = commandLineArgs[i + 1];
        if (flag == "-bf") {
            interpreter->Read(val); 
        }
        else if (flag == "-i") {
            interpreter->SetCommandLineInputs(val);
        }
    }
    return interpreter;
}

int main(int argc, char** argv) {

    const auto interpreter = ParseCommandLineArgs(argc, argv);
    interpreter->Run();

    std::cout << "Press any key to continue...";
}