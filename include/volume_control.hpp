#pragma once
#include <Arduino.h>
#include "config.hpp"
#include "serial_protocol.hpp"

class VolumeControl {
public:
    VolumeControl(SerialProtocol& serial);
    
    void initialize();
    void update();
    
    uint8_t getCurrentVolume() const;
    bool isMuted() const;
    void  toggleMute();
private:
    SerialProtocol& serialProtocol;
    uint8_t currentVolume = 50; // 0-100%
    uint8_t lastSentVolume = 0;
    bool muted = false;
    uint32_t lastReadTime = 0;
    
    void readVolume();
    void sendVolumeCommand();
};