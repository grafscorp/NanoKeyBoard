#ifndef I_SERIAL_DATA_SOURCE_HPP
#define I_SERIAL_DATA_SOURCE_HPP

#include <Arduino.h>

class ISerialDataSource
{
public:
    virtual  ~ISerialDataSource() = default;
protected:
    virtual const uint8_t getSerialData() const = 0;
    virtual const uint8_t getSerialCommand() const = 0;
    virtual void updateSerialData() = 0;
    friend class SerialProtocol;
};



#endif