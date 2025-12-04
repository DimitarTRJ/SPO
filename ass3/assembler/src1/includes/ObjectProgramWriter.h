#ifndef OBJECT_PROGRAM_WRITER_H
#define OBJECT_PROGRAM_WRITER_H

#include "AssemblerPass2.h"
#include <string>
#include <vector>

class ObjectProgramWriter {
public:
    ObjectProgramWriter(const std::string& programName, int startAddr, int programLength);

    std::string write(const std::vector<ObjectLine>& objectLines, const std::vector<ModificationRecord>& mods, int execStartAddr);

private:
    std::string programName;
    int startAddress;
    int programLength;

    std::string writeHeader() const;
    std::string writeTextRecords(const std::vector<ObjectLine>& objectLines) const;
    std::string writeModificationRecords(const std::vector<ModificationRecord>& mods) const;
    std::string writeEndRecord(int execStartAddr) const;
};

#endif
