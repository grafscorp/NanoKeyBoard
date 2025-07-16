// #include "serial_protocol.hpp"
// #include "button_handler.hpp"
// #include "display_manager.hpp"

// // Конструктор
// SerialProtocol::SerialProtocol(ButtonHandler& btnHandler, DisplayManager& displayMgr)
//     : buttonHandler(btnHandler), displayManager(displayMgr) {
    
//         loadConfiguration();
//         applyConfiguration(displayManager);

//     }

// /**
//  * @brief Initialize serial communication
//  */
// void SerialProtocol::init() {

//     resetProtocol();

//     rxHead = 0;
//     rxTail = 0;
//     rxCount = 0;
//     memset(rxBuffer, 0, sizeof(rxBuffer));
// }

// /**
//  * @brief Main processing loop
//  */
// void SerialProtocol::update() {
//     // Прием данных
//     while (Serial.available() && rxCount < sizeof(rxBuffer)) {
//         rxBuffer[rxHead] = Serial.read();
//         rxHead = (rxHead + 1) % sizeof(rxBuffer);
//         rxCount++;
//     }
    
//     // Обработка полученных данных
//     processIncomingData();
    
//     // Обработка таймаутов
//     const uint32_t currentTime = millis();
    
//     // Таймаут приема пакета
//     if (state == ProtocolState::Receiving && 
//         (currentTime - lastByteTime) > ProtocolConfig::ResponseTimeout) {
//         sendError(ProtocolConfig::ErrorCode::PacketTooLong);
//         resetProtocol();
//     }
    
//     // Таймаут подтверждения
//     if (state == ProtocolState::AwaitingAck && 
//         (currentTime - lastAckTime) > ProtocolConfig::AckTimeout) {
//         // Повторная отправка
//         sendPacket(true);
//         lastAckTime = currentTime;
//     }
// }

// void SerialProtocol::sendVolumeCommand(VolumeConfig::VolumeCommand command,
//                                        uint8_t value) {
//     currentTxPacket.command = static_cast<ProtocolConfig::Command>(command);
//     currentTxPacket.flags = 0;
//     currentTxPacket.lenght = 1;
//     currentTxPacket.data[0] = value;
//     sendPacket();
//                                        }

// /**
//  * @brief Process incoming bytes
//  */
// void SerialProtocol::processIncomingData() {
//     while (rxCount > 0) {
//         uint8_t byte = rxBuffer[rxTail];
//         rxTail = (rxTail + 1) % sizeof(rxBuffer);
//         rxCount--;
        
//         lastByteTime = millis();
        
//         switch (state) {
//             case ProtocolState::Idle:
//                 if (byte == ProtocolConfig::PacketStartMarker) {
//                     state = ProtocolState::Receiving;
//                     bytesReceived = 0;
//                     expectedLength = 0;
//                 }
//                 break;
                
//             case ProtocolState::Receiving:
//                 // Заголовок: [Command][Length][Flags]
//                 if (bytesReceived == 0) {
//                     currentRxPacket.command = static_cast<ProtocolConfig::Command>(byte);
//                 }
//                 else if (bytesReceived == 1) {
//                     currentRxPacket.lenght = byte;
//                     expectedLength = ProtocolConfig::HeaderSize + byte + 1; // +CRC
//                 }
//                 else if (bytesReceived == 2) {
//                     currentRxPacket.flags = byte;
//                 }
//                 // Данные
//                 else if (bytesReceived < expectedLength - 1) {
//                     if ((bytesReceived - ProtocolConfig::HeaderSize) < sizeof(currentRxPacket.data)) {
//                         currentRxPacket.data[bytesReceived - ProtocolConfig::HeaderSize] = byte;
//                     }
//                 }
//                 // CRC
//                 else if (bytesReceived == expectedLength - 1) {
//                     uint8_t calculatedCrc = calculateCrc(
//                         reinterpret_cast<uint8_t*>(&currentRxPacket),
//                         expectedLength - 1
//                     );
                    
//                     if (calculatedCrc == byte) {
//                         state = ProtocolState::Processing;
//                         processPacket();
//                     } else {
//                         sendError(ProtocolConfig::ErrorCode::ChecksumMismatch);
//                         resetProtocol();
//                     }
//                 }
                
//                 bytesReceived++;
                
//                 // Проверка переполнения
//                 if (bytesReceived >= ProtocolConfig::MaxPacketSize) {
//                     sendError(ProtocolConfig::ErrorCode::PacketTooLong);
//                     resetProtocol();
//                 }
//                 break;
                
//             default:
//                 // Игнорируем байты в других состояниях
//                 break;
//         }
//     }
// }

// /**
//  * @brief Handle complete packet
//  */
// void SerialProtocol::processPacket() {
//     // Проверяем необходимость подтверждения
//     bool ackRequired = (currentRxPacket.flags & ProtocolConfig::FlagAckRequired);
    
//     // Обработка команды
//     bool success = true;
//     uint8_t errorContext = 0;
    
//     switch (currentRxPacket.command) {
//         case ProtocolConfig::Command::SetButtonAction:
//             handleSetButtonAction();
//             break;
            
//         case ProtocolConfig::Command::SetDisplayText:
//             handleSetDisplayText();
//             break;
            
//         case ProtocolConfig::Command::SaveConfig:
//             handleSaveConfig();
//             break;
            
//         case ProtocolConfig::Command::ResetDevice:
//             handleResetDevice();
//             break;
            
//         case ProtocolConfig::Command::GetStatus:
//             handleGetStatus();
//             break;
            
//         default:
//             success = false;
//             errorContext = static_cast<uint8_t>(currentRxPacket.command);
//             sendError(ProtocolConfig::ErrorCode::InvalidCommand, errorContext);
//             break;
//     }
    
//     // Отправляем подтверждение при необходимости
//     if (ackRequired) {
//         sendAck(currentRxPacket.command, success);
//     }
    
//     resetProtocol();
// }

// /**
//  * @brief Send button event notification
//  */
// void SerialProtocol::sendButtonEvent(uint8_t buttonId, 
//                                      //ButtonHandler::ButtonState state, 
//                                      int state,
//                                      uint32_t duration) {
//     currentTxPacket.command = ProtocolConfig::Command::ButtonEvent;
//     currentTxPacket.flags = 0;
//     currentTxPacket.lenght = 6; // buttonId(1) + state(1) + duration(4)
    
//     currentTxPacket.data[0] = buttonId;
//     currentTxPacket.data[1] = static_cast<uint8_t>(state);
    
//     // Pack 32-bit duration
//     currentTxPacket.data[2] = (duration >> 24) & 0xFF;
//     currentTxPacket.data[3] = (duration >> 16) & 0xFF;
//     currentTxPacket.data[4] = (duration >> 8) & 0xFF;
//     currentTxPacket.data[5] = duration & 0xFF;
    
//     sendPacket();
// }

// /**
//  * @brief Send display state information
//  */
// void SerialProtocol::sendDisplayState(DisplayManager::DisplayState state) {
//     currentTxPacket.command = ProtocolConfig::Command::DisplayState;
//     currentTxPacket.flags = 0;
//     currentTxPacket.lenght = 1;
//     currentTxPacket.data[0] = static_cast<uint8_t>(state);
    
//     sendPacket();
// }

// void SerialProtocol::sendSystemError(uint8_t errorCode) {
//      currentTxPacket.command = ProtocolConfig::Command::ErrorReport;
//         currentTxPacket.flags = ProtocolConfig::FlagError;
//         currentTxPacket.lenght = 3;
//         currentTxPacket.data[0] = 0xFF; // Системная ошибка
//         currentTxPacket.data[1] = errorCode;
//         currentTxPacket.data[2] = millis() >> 24; // Старший байт времени
//         sendPacket(false);
// }

// void SerialProtocol::sendButtonConfiguration() {
//     currentTxPacket.command = ProtocolConfig::Command::ConfigAck;
//     currentTxPacket.flags = 0;
//     currentTxPacket.lenght = ButtonsConfig::BUTTONS_SIZE;
    
//     for (uint8_t i = 0; i < ButtonsConfig::BUTTONS_SIZE; i++) {
//         currentTxPacket.data[i] = buttonHandler.getButtonAction(i + 1);
//     }
    
//     sendPacket();
// }

// /**
//  * @brief Send packet
//  */
// void SerialProtocol::sendPacket(bool ackRequired) {
//     if (ackRequired) {
//         currentTxPacket.flags |= ProtocolConfig::FlagAckRequired;
//         state = ProtocolState::AwaitingAck;
//         lastAckTime = millis();
//     }
    
//     // Формируем пакет
//     uint8_t packetBuffer[ProtocolConfig::MaxPacketSize];
//     uint8_t idx = 0;
    
//     // Заголовок
//     packetBuffer[idx++] = ProtocolConfig::PacketStartMarker;
//     packetBuffer[idx++] = static_cast<uint8_t>(currentTxPacket.command);
//     packetBuffer[idx++] = currentTxPacket.lenght;
//     packetBuffer[idx++] = currentTxPacket.flags;
    
//     // Данные
//     for (uint8_t i = 0; i < currentTxPacket.lenght; i++) {
//         packetBuffer[idx++] = currentTxPacket.data[i];
//     }
    
//     // CRC
//     uint8_t crc = calculateCrc(packetBuffer, idx);
//     packetBuffer[idx++] = crc;
    
//     // Конечный маркер
//     packetBuffer[idx++] = ProtocolConfig::PacketEndMarker;
    
//     // Отправка
//     Serial.write(packetBuffer, idx);
// }

// /**
//  * @brief Send acknowledgement
//  */
// void SerialProtocol::sendAck(ProtocolConfig::Command originalCommand, bool success) {
//     currentTxPacket.command = ProtocolConfig::Command::ConfigAck;
//     currentTxPacket.flags = success ? 0 : ProtocolConfig::FlagError;
//     currentTxPacket.lenght = 1;
//     currentTxPacket.data[0] = static_cast<uint8_t>(originalCommand);
    
//     sendPacket(false);
// }

// /**
//  * @brief Send error report
//  */
// void SerialProtocol::sendError(ProtocolConfig::ErrorCode code, uint8_t context) {
//     currentTxPacket.command = ProtocolConfig::Command::ErrorReport;
//     currentTxPacket.flags = ProtocolConfig::FlagError;
//     currentTxPacket.lenght = 2;
//     currentTxPacket.data[0] = static_cast<uint8_t>(code);
//     currentTxPacket.data[1] = context;
    
//     sendPacket(false);
// }

// /**
//  * @brief Calculate CRC-8 checksum
//  */
// uint8_t SerialProtocol::calculateCrc(const uint8_t* data, uint8_t length) {
//     const uint8_t polynomial = 0x07;
//     uint8_t crc = 0;
    
//     for (uint8_t i = 0; i < length; i++) {
//         crc ^= data[i];
//         for (uint8_t j = 0; j < 8; j++) {
//             if (crc & 0x80) {
//                 crc = (crc << 1) ^ polynomial;
//             } else {
//                 crc <<= 1;
//             }
//         }
//     }
    
//     return crc;
// }

// /**
//  * @brief Handle SetButtonAction command
//  */
// void SerialProtocol::handleSetButtonAction() {
//     if (currentRxPacket.lenght != 2) {
//         sendError(ProtocolConfig::ErrorCode::InvalidCommand);
//         return;
//     }
    
//     uint8_t buttonId = currentRxPacket.data[0];
//     uint8_t actionId = currentRxPacket.data[1];
    
//     // Валидация ID кнопки
//     if (buttonId < 1 || buttonId > ButtonsConfig::BUTTONS_SIZE) {
//         sendError(ProtocolConfig::ErrorCode::InvalidButtonId, buttonId);
//         return;
//     }
    
//     // Валидация ID действия
//     if (actionId == 0 || actionId > 0x7F) {
//         sendError(ProtocolConfig::ErrorCode::InvalidActionId, actionId);
//         return;
//     }
    
//     // Применение действия к кнопке
//     currentConfig.buttonActions[buttonId - 1] = actionId;
    
//     // Обновление дисплея
//     String message = "Button " + String(buttonId);
//     String action = "Action: " + String(actionId);
//     displayManager.showMessage(message, action, 2000, 10);
    
//     // Отправка подтверждения
//     sendAck(currentRxPacket.command);
// }

// /**
//  * @brief Handle SetDisplayText command
//  */
// void SerialProtocol::handleSetDisplayText() {
//     if (currentRxPacket.lenght < 3) {
//         sendError(ProtocolConfig::ErrorCode::InvalidCommand);
//         return;
//     }
    
//     // Распаковка длительности
//     uint16_t duration = (static_cast<uint16_t>(currentRxPacket.data[0]) << 8) | 
//                          currentRxPacket.data[1];
//     uint8_t priority = currentRxPacket.data[2];
    
//     // Поиск разделителя строк
//     uint8_t textStart = 3;
//     uint8_t textEnd = textStart;
//     while (textEnd < currentRxPacket.lenght && currentRxPacket.data[textEnd] != '\0') {
//         textEnd++;
//     }
    
//     if (textEnd >= currentRxPacket.lenght) {
//         sendError(ProtocolConfig::ErrorCode::InvalidCommand);
//         return;
//     }
    
//     // Извлечение первой строки
//     String line1(reinterpret_cast<char*>(currentRxPacket.data + textStart));
    
//     // Поиск начала второй строки
//     uint8_t secondLineStart = textEnd + 1;
//     if (secondLineStart >= currentRxPacket.lenght) {
//         // Только одна строка
//         displayManager.showMessage(line1, "", duration, priority);
//         return;
//     }
    
//     // Извлечение второй строки
//     String line2(reinterpret_cast<char*>(currentRxPacket.data + secondLineStart));
//     displayManager.showMessage(line1, line2, duration, priority);
// }

// /**
//  * @brief Handle SaveConfig command
//  */
// void SerialProtocol::handleSaveConfig() {
//     // Сохраняем конфигурацию
//     saveConfiguration();
    
//     // Применяем новые настройки
//     applyConfiguration(displayManager);
    
//     // Подтверждение
//     displayManager.showMessage("Configuration", "Saved", 2000, 10);
//     sendAck(currentRxPacket.command);
// }

// /**
//  * @brief Handle ResetDevice command
//  */
// void SerialProtocol::handleResetDevice() {
//     // Подтверждение перед сбросом
//     sendAck(currentRxPacket.command);
    
//     // Даем время на отправку подтверждения
//     delay(50);
    
//     // Программный сброс
//     asm volatile ("jmp 0");
// }

// /**
//  * @brief Handle GetStatus command
//  */
// void SerialProtocol::handleGetStatus() {
//     // // 1. Отправляем состояние дисплея
//     // sendDisplayState(displayManager.getCurrentState());
    
//     // // 2. Отправляем конфигурацию кнопок
//     // currentTxPacket.command = ProtocolConfig::Command::ConfigAck;
//     // currentTxPacket.flags = 0;
//     // currentTxPacket.lenght = ButtonsConfig::BUTTONS_SIZE;
    
//     // for (uint8_t i = 0; i < ButtonsConfig::BUTTONS_SIZE; i++) {
//     //     currentTxPacket.data[i] = currentConfig.buttonActions[i];
//     // }
    
//     // sendPacket();
    
//     // // 3. Отправляем версию прошивки
//     // const uint8_t firmwareVersion[] = {1, 0, 0}; // Major, Minor, Patch
//     // currentTxPacket.command = ProtocolConfig::Command::ConfigAck;
//     // currentTxPacket.flags = 0;
//     // currentTxPacket.lenght = sizeof(firmwareVersion);
//     // memcpy(currentTxPacket.data, firmwareVersion, sizeof(firmwareVersion));
    
//     // sendPacket();

//     // 1. Send display state
//     // sendDisplayState(displayManager.getCurrentState());
    
//     // // 2. Send button configuration
//     // sendButtonConfiguration();
    
//     // // 3. Send firmware version
//     // const uint8_t firmwareVersion[] = {1, 0, 0}; // Major, Minor, Patch
//     // currentTxPacket.command = ProtocolConfig::Command::ConfigAck;
//     // currentTxPacket.flags = 0;
//     // currentTxPacket.lenght = sizeof(firmwareVersion);
//     // memcpy(currentTxPacket.data, firmwareVersion, sizeof(firmwareVersion));
    
//     // sendPacket();
    
//     // // 4. Send system state
//     // currentTxPacket.command = ProtocolConfig::Command::ConfigAck;
//     // currentTxPacket.flags = 0;
//     // currentTxPacket.lenght = 2;
//     // currentTxPacket.data[0] = displayManager.getCurrentBrightness();
//     // currentTxPacket.data[1] = static_cast<uint8_t>(SystemState::getCurrentState());
    
//     // sendPacket();

//         // Упрощенная отправка статуса
//     currentTxPacket.command = ProtocolConfig::Command::GetStatus;
//     currentTxPacket.flags = 0;
//     currentTxPacket.lenght = 4;
    
//     currentTxPacket.data[0] = static_cast<uint8_t>(displayManager.getCurrentState());
//     currentTxPacket.data[1] = displayManager.getCurrentBrightness();
//     currentTxPacket.data[2] = static_cast<uint8_t>(SystemState::getCurrentState());
//     currentTxPacket.data[3] = ButtonsConfig::BUTTONS_SIZE;
    
//     sendPacket();
    
// }
// /**
//  * @brief Reset protocol state machine
//  */
// void SerialProtocol::resetProtocol() {
//     state = ProtocolState::Idle;
//     bytesReceived = 0;
//     expectedLength = 0;
//     memset(&currentRxPacket, 0, sizeof(Packet));
// }
