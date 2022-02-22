#include "BrainfuckInterpreter.h"

BrainfuckInterpreter::BrainfuckInterpreter() :
    m_MemoryPointer(512),
    m_InstructionPointer(0),
    m_InputPointer(0),
    m_UseCommandLineInputs(false),
    m_OutputBuffer(""),
    m_Memory(s_StartingCapacity, 0)
{ }

void BrainfuckInterpreter::Clear() {
    
    m_Code = "";
    m_OutputBuffer = "";
    m_Memory.resize(s_StartingCapacity);
    std::fill(m_Memory.begin(), m_Memory.end(), 0);
    m_MemoryPointer = 512;
    m_InstructionPointer = 0;
}

void BrainfuckInterpreter::SetCode(std::string_view code) {
    m_Code = code;
}

std::string BrainfuckInterpreter::GetCode() {
    return m_Code;
}

void BrainfuckInterpreter::SetCommandLineInputs(std::string inputs) {
    m_UseCommandLineInputs = true;
    m_Inputs = inputs;
}

void BrainfuckInterpreter::Read(std::string_view path) {
    std::ifstream fileStream(std::string(path), std::fstream::in);
    std::stringstream stringStream;
    stringStream << fileStream.rdbuf();
    m_Code = stringStream.str();
}

void BrainfuckInterpreter::LoopStart() {
    if (m_Memory[m_MemoryPointer] == 0) {
        int loopDepth = 0;
        for (; m_InstructionPointer < m_Code.length(); m_InstructionPointer++) {
            if (m_Code[m_InstructionPointer] == '[') {
                loopDepth++;
            }
            else if (m_Code[m_InstructionPointer] == ']') {
                if (!--loopDepth) {
                    break;
                }
            }
        }
        if (loopDepth < 0) {
            throw std::logic_error("Unmatched closing bracket");
        }
        else if (loopDepth > 0) {
            throw std::logic_error("Unmatched opening bracket");
        }
    }
    else {
        m_LoopBeginPointers.push(m_InstructionPointer);
    }
}

void BrainfuckInterpreter::LoopEnd() {
    if (m_Memory[m_MemoryPointer] == 0) {
        if (m_LoopBeginPointers.empty()) {
            throw std::logic_error("Unmatched Closing Bracket");
        }
        m_LoopBeginPointers.pop();
        return;
    }

    if (m_LoopBeginPointers.empty()) {
        throw std::logic_error("Unmatched Closing Bracket");
    }
    m_InstructionPointer = m_LoopBeginPointers.top();
}

void BrainfuckInterpreter::Run() {
    while(m_InstructionPointer < m_Code.length()) {
        auto currentToken = m_Code[m_InstructionPointer];

        switch (currentToken) {
        case '+':
            m_Memory[m_MemoryPointer]++;
            break;
        case '-':
            m_Memory[m_MemoryPointer]--;
            break;
        case '<':
            m_MemoryPointer--;
            break;
        case '>':
            if (m_MemoryPointer == (m_Memory.size() - 1)) {
                m_Memory.resize(m_Memory.size() * 2);
            }
            m_MemoryPointer++;
            break;
        case '[':
            LoopStart();
            break;
        case ']':
            LoopEnd();
            break;
        case ',':
            if (m_UseCommandLineInputs) {
                ::Clear();
                std::cout << "Input: ";
                m_Memory[m_MemoryPointer] = std::getchar();
                ::Clear();
                std::cout << m_OutputBuffer;
            }
            else {
                if (m_InputPointer == m_Inputs.length()) {
                    throw std::invalid_argument("Number of Inputs supplied is not valid");
                }
                m_Memory[m_MemoryPointer] = m_Inputs[m_InputPointer++];
            }
            break;
        case '.':
            m_OutputBuffer.push_back(m_Memory[m_MemoryPointer]);
            std::cout << m_Memory[m_MemoryPointer];
            break;
        default:
            break;
        }

        m_InstructionPointer++;
    }
}