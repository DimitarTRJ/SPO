
#include "Device.h"
#include "FileDevice.h"
#include <fstream>

#include <iostream>
#include <vector>
#include <array>
#include <stdexcept>
#include <cstring>


    FileDevice::FileDevice(const std::string& filename) : file(filename, std::ios::in | std::ios::out | std::ios::binary)
    {
        // If file does not exist, create it
        if (!file.is_open()) 
        {
            file.clear();
            file.open(filename, std::ios::out | std::ios::binary);
            file.close();
            file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
        }
    }

    bool FileDevice::test(){
        return file.good();
    }

    uint8_t FileDevice::read(){
        char c = 0;
        file.get(c);
        return static_cast<uint8_t>(c);
    }

    void FileDevice::write(uint8_t value){
        char c = static_cast<char>(value);
        file.put(c);
        file.flush();
    }
