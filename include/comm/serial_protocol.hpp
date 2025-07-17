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

private:
    //Aлгоритм CRC-8 — простую контрольную сумму для обнаружения ошибок в последовательности байт.
    //Target - Вычислить 1-байтовую CRC-8 контрольную сумму для переданных данных, используя полином 0x07, что соответствует стандартному CRC-8 (также известному как CRC-8-ATM или CRC-8-ITU).
    uint8_t crc8(const uint8_t *data, size_t len);

};





#endif