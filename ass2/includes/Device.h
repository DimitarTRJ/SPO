#ifndef DEVICE_H
#define DEVICE_H

#include <cstdint>

class Device{
public:
    virtual ~Device() = default;

    virtual bool test() { return true; }
    virtual uint8_t read() { return 0; }
    virtual void write(uint8_t) {}
};

#endif // DEVICE_H