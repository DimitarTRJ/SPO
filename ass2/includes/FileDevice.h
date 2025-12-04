#ifndef FILEDEVICE_H
#define FILEDEVICE_H

#include "Device.h"
#include <fstream>
#include <string>

class FileDevice : public Device {
public:
    FileDevice(const std::string& filename);

    bool test() override;
    uint8_t read() override;
    void write(uint8_t value) override;

private:
    std::fstream file;
};

#endif // FILEDEVICE_H
