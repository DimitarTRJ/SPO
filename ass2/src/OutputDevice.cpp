
#include "OutputDevice.h"
#include "Device.h"
#include <ostream>

#include <iostream>
#include <vector>
#include <array>
#include <stdexcept>
#include <cstring>

    OutputDevice::OutputDevice(std::ostream& out) : output(out) {}

    void OutputDevice::write(uint8_t value) {
        output.put(static_cast<char>(value));
        output.flush();
    }



//#endif