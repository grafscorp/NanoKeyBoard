#ifndef SERIAL_PROTOCOL_HPP
#define SERIAL_PROTOCOL_HPP

#include <Arduino.h>
#include "config.hpp"
#include <RingBuf.h>
#include "eeprom_manager.hpp"
#include "display_manager.hpp"
#include "system_state.hpp"
// Структура для хранения конфигурации в EEPROM
struct SystemConfig {
    uint8_t buttonActions[ButtonsConfig::BUTTONS_SIZE];
    uint8_t displayBrightness;
    uint16_t sleepTimeout;
    uint8_t checksum;
};
namespace {
    // Текущая конфигурация системы
    SystemConfig currentConfig;
    
    // Адрес конфигурации в EEPROM
    constexpr int EEPROM_CONFIG_ADDR = 0;
    
    /**
     * @brief Вычисление контрольной суммы конфигурации
     */
    uint8_t calculateConfigChecksum(const SystemConfig& config) {
        uint8_t sum = 0;
        const uint8_t* data = reinterpret_cast<const uint8_t*>(&config);
        for (size_t i = 0; i < sizeof(SystemConfig) - 1; i++) {
            sum ^= data[i];
        }
        return sum;
    }
    //     /**
    //  * @brief Send volume command to PC
    //  * @param command Volume command type
    //  * @param value Volume value (0-100)
    //  */
    
    /**
     * @brief Загрузка конфигурации из EEPROM
     */
    bool loadConfiguration() {
        EEPROM.get(EEPROM_CONFIG_ADDR, currentConfig);
        
        // Проверка контрольной суммы
        if (calculateConfigChecksum(currentConfig) != currentConfig.checksum) {
            // Конфигурация повреждена, устанавливаем значения по умолчанию
            for (uint8_t i = 0; i < ButtonsConfig::BUTTONS_SIZE; i++) {
                currentConfig.buttonActions[i] = static_cast<uint8_t>(
                    static_cast<ButtonsConfig::ButtonDefault>(i + 1)
                );
            }
            currentConfig.displayBrightness = 255; // Максимальная яркость
            currentConfig.sleepTimeout = DisplayConfig::ScreenTimeout;
            currentConfig.checksum = calculateConfigChecksum(currentConfig);
            
            return false;
        }
        return true;
    }
    
    /**
     * @brief Сохранение конфигурации в EEPROM
     */
    void saveConfiguration() {
        currentConfig.checksum = calculateConfigChecksum(currentConfig);
        EEPROM.put(EEPROM_CONFIG_ADDR, currentConfig);
    }
    
    /**
     * @brief Применение конфигурации к системе
     */
    void applyConfiguration(DisplayManager& display) {
        // Применяем настройки дисплея
        display.setBrightness(currentConfig.displayBrightness);

    }
}

// Предварительные объявления для дружественных классов
class ButtonHandler;
class DisplayManager;

/**
 * @class SerialProtocol
 * @brief Binary protocol handler for reliable device-PC communication
 */
class SerialProtocol
{
public:
    /**
     * @struct Packet
     * @brief Protocol packet structure
     */
    struct Packet
    {
        ProtocolConfig::Command command;
        uint8_t flags;
        uint8_t lenght;
        uint8_t data[ProtocolConfig::MaxPacketSize - ProtocolConfig::HeaderSize -2];
    };

    SerialProtocol(ButtonHandler& btnHandler, DisplayManager& displayManager);

    /**
     * @brief Initialize serial communication
     */
    void init();
        
    /**
     * @brief Main processing loop
     */
    void update();
    /**
     * @brief Send volume command to PC
     * @param command Volume command type
     * @param value Volume value (0-100)
     */
    void sendVolumeCommand(VolumeConfig::VolumeCommand command, uint8_t value = 0);
    /**
     * @brief Send button event notification
     * @param buttonId Physical button ID
     * @param state Button state
     * @param duration Press duration
     */
    void sendButtonEvent(uint8_t buttonId, 
        //ButtonHandler::ButtonState state, 
                        int state, 
                         uint32_t duration);
    
    
                         /**
     * @brief Send display state information
     * @param state Current display state
     */
    void sendDisplayState(DisplayManager::DisplayState state);
    /**
     * @brief Send system error report
     * @param errorCode System-specific error code
     */
    void sendSystemError(uint8_t errorCode);
        /**
     * @brief Send current button configuration
     */
    void sendButtonConfiguration();
private:
    // Ссылки на другие модули
    ButtonHandler& buttonHandler;
    DisplayManager& displayManager;

        // Буферизация
    // RingBuf<uint8_t, 128> rxBuffer;
    Packet currentTxPacket;
    Packet currentRxPacket;
        uint8_t rxBuffer[128];
    uint8_t rxHead = 0;
    uint8_t rxTail = 0;
    uint8_t rxCount = 0;

    // Состояние протокола
    enum class ProtocolState {
        Idle,
        Receiving,
        AwaitingAck,
        Processing
    } state = ProtocolState::Idle;

    uint32_t lastByteTime = 0;
    uint32_t lastAckTime = 0;
    uint8_t expectedLength = 0;
    uint8_t bytesReceived = 0;

    /**
     * @brief Process incoming bytes
     */
    void processIncomingData();

    /**
     * @brief Handle complete packet
     */
    void processPacket();

    /**
     * @brief Send packet
     * @param ackRequired Set ACK flag
     */
    void sendPacket(bool ackRequired = false);


    /**
     * @brief Send acknowledgement packet
     * @param originalCommand Original command ID
     * @param success Operation status
     */
    void sendAck(ProtocolConfig::Command originalCommand, bool success = true);

    /**
     * @brief Send error report
     * @param code Error code
     * @param context Additional context
     */
    void sendError(ProtocolConfig::ErrorCode code, uint8_t context = 0);

    /**
     * @brief Calculate CRC8 checksum
     * @param data Data buffer
     * @param length Data length
     * @return CRC8 value
     */
    uint8_t calculateCrc(const uint8_t* data, uint8_t length);

    /**
     * @brief Handle SetButtonAction command
     */
    void handleSetButtonAction();

    /**
     * @brief Handle SetDisplayText command
     */
    void handleSetDisplayText();

    void handleSaveConfig();
    
    /**
     * @brief Handle ResetDevice command
     */
    void handleResetDevice();
    
    /**
     * @brief Handle GetStatus command
     */
    void handleGetStatus();
    
    /**
     * @brief Reset protocol state machine
     */
    void resetProtocol();


};



#endif