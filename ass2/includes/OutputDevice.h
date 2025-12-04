#ifndef OUTPUT_DEVICE_H
#define OUTPUT_DEVICE_H

#include "Device.h"
#include <ostream>

class OutputDevice : public Device {
public:
    OutputDevice(std::ostream& out);

    void write(uint8_t value) override;

private:
    std::ostream& output;
};

#endif // OUTPUTDEVICE_H
