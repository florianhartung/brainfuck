#pragma once

#include <string_view>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>
#include <cstdio>
#include <iostream>
#include <functional>
#include <unordered_map>
#include <unordered_set>

#include "Util.h"

class BrainfuckInterpreter {
public:
    BrainfuckInterpreter();

public:
    void Run();
    void Clear();
    void SetCode(std::string_view code);
    void Read(std::string_view path);

public:
    std::string GetCode();
    void SetCommandLineInputs(std::string inputs);

private:
    void LoopStart();
    void LoopEnd();

private:
    static constexpr uint16_t s_StartingCapacity = 1024;

    static const std::unordered_set<char> s_BrainfuckInstructions;

    std::unordered_map<char, std::function<void()>> m_TokenToAction;

    std::string m_Code;
    std::vector<uint8_t> m_Memory;
    std::stack<uint32_t> m_LoopBeginPointers;
    std::string m_OutputBuffer;
    uint32_t m_InstructionPointer;
    uint32_t m_MemoryPointer;

    bool m_UseCommandLineInputs;
    std::string m_Inputs;
    uint32_t m_InputPointer;
};