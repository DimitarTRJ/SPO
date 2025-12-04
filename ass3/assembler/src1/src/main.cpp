#include "Lexer.h"
#include "Parser.h"
#include "AssemblerPass1.h"
#include "AssemblerPass2.h"
#include "ObjectProgramWriter.h"
#include <fstream>
#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: assembler <sourcefile>\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file) { std::cerr << "Cannot open file\n"; return 1; }

    std::string source((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    Lexer lexer(source);
    Parser parser(lexer);

    std::vector<SourceLine> lines;
    while (!parser.eof()) {
        SourceLine line = parser.parseSourceLine();
        lines.push_back(line);
    }

    //std::cerr << "Parsed lines: " << lines.size() << "\n";

    try {
        AssemblerPass1 pass1;
        pass1.run(lines);

        AssemblerPass2 pass2(pass1.getSymbolTable(), pass1.getStartAddress());
        pass2.run(lines);

        ObjectProgramWriter writer(pass1.getProgramName(), pass1.getStartAddress(), pass1.getProgramLength());

        int execStart = pass1.getStartAddress();
        for (const auto& l : lines) {
            if (l.opcode == "END" && !l.operand.empty()) {
                execStart = pass1.getSymbolTable().at(l.operand);
                break;
            }
        }

        std::string obj = writer.write(pass2.getObjectLines(), pass2.getModRecords(), execStart);
        std::cout << obj;

    } catch (const std::exception& e) {
        std::cerr << "Assembly error: " << e.what() << "\n";
        return 1;
    }
}
