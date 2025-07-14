#include "volume_control.hpp"

VolumeControl::VolumeControl(SerialProtocol& serial) 
    : serialProtocol(serial) {}

void VolumeControl::initialize() {
    pinMode(HardwareConfig::VolumePotPin, INPUT);
}

void VolumeControl::update() {
    const uint32_t currentTime = millis();
    
    if (currentTime - lastReadTime >= HardwareConfig::VolumeReadInterval) {
        readVolume();
        lastReadTime = currentTime;
    }
}

void VolumeControl::readVolume() {
    // Чтение аналогового значения (0-1023)
    int rawValue = analogRead(HardwareConfig::VolumePotPin);
    
    // Преобразование в проценты (0-100%)
    uint8_t newVolume = map(rawValue, 0, 1023, 0, 100);
    
    // Фильтрация шумов
    if (abs(newVolume - currentVolume) > HardwareConfig::VolumeChangeThreshold) {
        currentVolume = newVolume;
        
        // Отправка команды только при существенном изменении
        if (abs(currentVolume - lastSentVolume) >= 5) {
            sendVolumeCommand();
            lastSentVolume = currentVolume;
        }
    }
}

void VolumeControl::sendVolumeCommand() {
    if (muted) return; // Не отправляем, если звук выключен
    
    serialProtocol.sendVolumeCommand(
        VolumeConfig::VolumeCommand::SetVolume,
        currentVolume
    );
}

uint8_t VolumeControl::getCurrentVolume() const {
    return currentVolume;
}

bool VolumeControl::isMuted() const {
    return muted;
}

void VolumeControl::toggleMute() {

        muted = !muted;
    
    if (muted) {
        // Отправляем команду выключения звука
        serialProtocol.sendVolumeCommand(
            VolumeConfig::VolumeCommand::MuteToggle,
            0
        );
    } else {
        // Восстанавливаем предыдущую громкость
        sendVolumeCommand();
    }
}
