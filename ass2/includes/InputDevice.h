#ifndef INPUT_DEVICE_H
#define INPUT_DEVICE_H

#include "Device.h"
#include <istream>

class InputDevice : public Device {
public:
    InputDevice(std::istream& in);

    uint8_t read() override;

private:
    std::istream& input;
};

#endif