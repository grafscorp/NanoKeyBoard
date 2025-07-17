#ifndef I_SERIAL_DATA_SOURCE_HPP
#define I_SERIAL_DATA_SOURCE_HPP

#include <Arduino.h>

class ISerialDataSource
{
public:
    virtual uint8_t getSerialData() = 0;
    virtual  ~ISerialDataSource() = default;
};




#endif