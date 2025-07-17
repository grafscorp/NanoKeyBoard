#ifndef SERIAL_PROTOCOL_HPP
#define SERIAL_PROTOCOL_HPP

#include <Arduino.h>
#include "config.hpp"


class SerialProtocol
{
public:
    SerialProtocol();
    void init();
    void update();
    void sendPacket(const uint8_t cmd, const uint8_t data);

};





#endif