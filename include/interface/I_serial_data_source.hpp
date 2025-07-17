#ifndef I_SERIAL_DATA_SOURCE_HPP
#define I_SERIAL_DATA_SOURCE_HPP

#include <Arduino.h>

class ISerialDataSource
{
public:
    virtual const uint8_t getSerialData() const = 0;
    virtual  ~ISerialDataSource() = default;
};




#endif