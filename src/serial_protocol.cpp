
#include "comm/serial_protocol.hpp"


SerialProtocol::SerialProtocol() {
    
}

void SerialProtocol::init()
{
    
}

void SerialProtocol::update()
{

}

void SerialProtocol::sendData(const ISerialDataSource &source)
{

    sendPacket(source.getSerialCommand(), source.getSerialData());
}

void SerialProtocol::sendPacket(const uint8_t cmd, const uint8_t data) {
    uint8_t packet[ProtocolConfig::PACKET_SIZE];
    packet[0] = ProtocolConfig::START_BYTE;

    packet[1] = cmd + data;
    packet[2] = cmd;
    packet[3] = data;
    packet[4] = crc8(&packet[1], 3);

    packet[5] = ProtocolConfig::END_BYTE;

    Serial.write(packet, ProtocolConfig::PACKET_SIZE);
}


uint8_t SerialProtocol::crc8(const uint8_t *data, size_t len)
{
    uint8_t crc = 0x00;
    for (uint8_t i = 0; i < len; i++) {
        crc ^= data[i];
        /*В этом цикле происходит побитовая обработка каждого из 8 бит текущего байта:
        crc & 0x80: проверяется, установлен ли старший бит (бит 7).
        Если да, происходит сдвиг влево и XOR с полиномом 0x07.
        Если нет, просто сдвигаем влево.
        */
        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ 0x07;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}