#include <iostream>
#include <fstream>
#include "Machine.h"

int main() {
    Machine m;

    std::cout << "=== Simple SIC/XE Machine Test ===\n";

    std::ifstream file("test.obj", std::ios::binary);
    if (!file) {
        std::cout << "Could not open test.obj. Make sure it is in the same folder.\n";
        return 1;
    }

    if (!m.loadSection(file)) {
        std::cout << "Failed to load section!\n";
        return 1;
    }

    std::cout << "Program loaded successfully.\n";
    std::cout << "Starting at PC = 0x" << std::hex << m.getPC() << "\n";
    std::cout << "Type 's' to execute one step, 'r' to run 10 steps, 'q' to quit.\n\n";

    while (true) {
        std::cout << "> ";
        char cmd;
        std::cin >> cmd;

        if (cmd == 'q') {
            std::cout << "Exiting.\n";
            break;
        }
        else if (cmd == 's') {
            m.execute();
        }
        else if (cmd == 'r') {
            for (int i = 0; i < 10 && !m.halted; i++) {
                m.execute();
            }
        }
        else if (cmd == 'a') {
            while (!m.halted) {
                m.execute();
            }
            std::cout << "Program halted.\n";
        }
        else {
            std::cout << "Unknown command.\n";
            continue;
        }

        // Print registers after each step
        std::cout << "PC: 0x" << std::hex << m.getPC() << "\n";
        std::cout << "A : 0x" << std::hex << m.getA()  << "\n";
        std::cout << "X : 0x" << std::hex << m.getX()  << "\n";
        std::cout << "L : 0x" << std::hex << m.getL()  << "\n";
        std::cout << "B : 0x" << std::hex << m.getB()  << "\n";
        std::cout << "S : 0x" << std::hex << m.getS()  << "\n";
        std::cout << "T : 0x" << std::hex << m.getT()  << "\n";
        std::cout << "F : 0x" << std::hex << m.getF()  << "\n";
        std::cout << "SW : 0x" << std::hex << m.getSW()  << "\n";
        std::cout << "---------------------------------------\n";
    }

    return 0;
}
