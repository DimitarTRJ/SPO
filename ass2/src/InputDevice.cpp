
#include "InputDevice.h"
#include "Device.h"
#include <istream>
#include <iostream>
#include <vector>
#include <array>
#include <stdexcept>
#include <cstring>


    InputDevice::InputDevice(std::istream& in) : input(in) {}

    uint8_t InputDevice::read() {
        int c = input.get();
        if (c == EOF) return 0;
        return static_cast<uint8_t>(c);
    }


//#endif