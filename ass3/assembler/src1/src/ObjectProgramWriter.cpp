#include "ObjectProgramWriter.h"
#include "AssemblerUtils.h"
#include <sstream>
#include <iomanip>

ObjectProgramWriter::ObjectProgramWriter(const std::string& progName, int startAddr,int progLen)
    : programName(progName),
      startAddress(startAddr),
      programLength(progLen) {}

std::string ObjectProgramWriter::write(
    const std::vector<ObjectLine>& objectLines,
    const std::vector<ModificationRecord>& mods,
    int execStartAddr) {

    std::ostringstream out;
    out << writeHeader();
    out << writeTextRecords(objectLines);
    out << writeModificationRecords(mods);
    out << writeEndRecord(execStartAddr);
    return out.str();
}

std::string ObjectProgramWriter::writeHeader() const {
    std::ostringstream oss;
    oss << "H^"
        << std::left << std::setw(6) << std::setfill(' ')
        << programName.substr(0, 6)
        << "^" << toHex(startAddress, 6)
        << "^" << toHex(programLength, 6)
        << "\n";
    return oss.str();
}

/*std::string ObjectProgramWriter::writeTextRecords(
    const std::vector<ObjectLine>& objectLines) const {

    constexpr int MAX_TEXT_BYTES = 0x1E;

    std::ostringstream oss;

    int recordStart = -1;
    int recordLength = 0;
    std::string recordData;
    int lastEndAddress = -1;

    auto flush = [&]() {
        if (recordLength > 0) 
        {
            oss << "T^"  << toHex(recordStart, 6) << "^" << toHex(recordLength, 2)
                << "^" << recordData << "\n";
        }
        recordStart = -1;
        recordLength = 0;
        recordData.clear();
        lastEndAddress = -1;
    };

    for (const auto& obj : objectLines) {
        if (obj.objectCode.empty())
            continue;

        int objBytes = obj.objectCode.length() / 2;

        bool discontinuity =
            (recordStart != -1 && obj.address != lastEndAddress);

        bool overflow =
            (recordLength + objBytes > MAX_TEXT_BYTES);

        if (recordStart == -1 || discontinuity || overflow) {
            flush();
            recordStart = obj.address;
        }

        recordData += obj.objectCode;
        recordLength += objBytes;
        lastEndAddress = obj.address + objBytes;
    }

    flush();
    return oss.str();
} */

std::string ObjectProgramWriter::writeTextRecords(const std::vector<ObjectLine>& objectLines) const {

    constexpr int MAX_TEXT_BYTES = 0x1E; // 30 bytes

    std::ostringstream oss;

    int recordStart = -1;
    int recordLength = 0;
    std::string recordData;
    int expectedAddr = -1; // next expected address for continuity

    auto flush = [&]() {
        if (recordLength > 0) {
            oss << "T^" << toHex(recordStart, 6) << "^" 
                << toHex(recordLength, 2) << "^" 
                << recordData << "\n";
        }
        recordStart = -1;
        recordLength = 0;
        recordData.clear();
        expectedAddr = -1;
    };

    for (const auto& obj : objectLines) {
        if (obj.objectCode.empty()) continue;

        int objBytes = static_cast<int>(obj.objectCode.length() / 2);

        bool needFlush = false;

        if (recordStart == -1) {
            needFlush = false;
        } else if (obj.address != expectedAddr) {
            needFlush = true;
        } else if (recordLength + objBytes > MAX_TEXT_BYTES) {
            needFlush = true;
        }

        if (needFlush) flush();

        if (recordStart == -1) recordStart = obj.address;

        recordData += obj.objectCode;
        recordLength += objBytes;
        expectedAddr = obj.address + objBytes;
    }

    // Flush last record
    flush();

    return oss.str();
}



std::string ObjectProgramWriter::writeModificationRecords(
    const std::vector<ModificationRecord>& mods) const {

    std::ostringstream oss;
    for (const auto& m : mods) 
    {
        oss << "M^" << toHex(m.address, 6) << "^" << toHex(m.length, 2) << "\n";
    }
    return oss.str();
}

std::string ObjectProgramWriter::writeEndRecord(int execStartAddr) const {
    std::ostringstream oss;
    oss << "E^" << toHex(execStartAddr, 6) << "\n";
    return oss.str();
}
