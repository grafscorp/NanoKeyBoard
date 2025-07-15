#include <Arduino.h>
#include "button_handler.hpp"
#include "display_manager.hpp"
#include "serial_protocol.hpp"
#include "eeprom_manager.hpp"
#include "system_state.hpp"
#include "volume_control.hpp"

ButtonHandler buttonHandler;
DisplayManager displayManager;
SerialProtocol serialProtocol(buttonHandler, displayManager);
VolumeControl volumeControl(serialProtocol);
EEPROMManager::SystemConfig systemConfig;

bool initializeConfiguration() {
    EEPROMManager::initialize();
    if (!EEPROMManager::load(systemConfig)) {
        for (uint8_t i = 0; i < ButtonsConfig::BUTTONS_SIZE; i++) {
            systemConfig.buttonActions[i] = static_cast<uint8_t>(
                static_cast<ButtonsConfig::ButtonDefault>(i + 1));
        }
        systemConfig.displayBrightness = 255;
        systemConfig.sleepTimeout = DisplayConfig::ScreenTimeout;
        EEPROMManager::save(systemConfig);
        return false;
    }
    return true;
}

void applyConfiguration() {
    for (uint8_t i = 0; i < ButtonsConfig::BUTTONS_SIZE; i++) {
        buttonHandler.setButtonAction(i + 1, systemConfig.buttonActions[i]);
    }
    displayManager.setBrightness(systemConfig.displayBrightness);
}

void initializePeripherals() {
    SystemState::initialize();
    bool configValid = initializeConfiguration();
    
    buttonHandler.init();
    volumeControl.initialize();
    
    if (!displayManager.init()) {
        SystemState::setState(SystemState::ErrorState);
        serialProtocol.sendSystemError(1);
    } else {
        applyConfiguration();
    }
    
    serialProtocol.init();
    
    if (SystemState::getCurrentState() != SystemState::ErrorState) {
        if (!configValid) {
            displayManager.showMessage("System Ready", "Default Config", 3000, 10);
        } else {
            displayManager.showMessage("System Ready", "v1.0", 2000, 10);
        }
    }
}

void handleButtonEvents() {
    if (!buttonHandler.hasEvent()) return;
    
    auto event = buttonHandler.getEvent();
    SystemState::updateActivity();
    
    if (SystemState::isState(SystemState::SleepMode)) {
        SystemState::setState(SystemState::NormalOperation);
        displayManager.wakeUp();
        return;
    }
    
    if (event.state == ButtonHandler::Pressed) {
        buttonHandler.executeButtonAction(event.buttonID);
    }
    
    serialProtocol.sendButtonEvent(
        event.buttonID,
        static_cast<uint8_t>(event.state),
        event.duration
//     serialProtocol.sendButtonEvent(
//         event.buttonID,
//         static_cast<uint8_t>(event.state),
//         event.duration
//     );
    
//     if (event.state == ButtonHandler::Pressed) {
//         displayManager.showMessage(
//             "Btn " + String(event.buttonID),
//             "Pressed",
//             500,
//             5
//         );
//     } else if (event.state == ButtonHandler::Held) {
//         displayManager.showMessage(
//             "Btn " + String(event.buttonID),
//             "Long press",
//             1000,
//             5
//         );
//     }
// }

// void handleSystemStates() {
//     static uint32_t lastStateCheck = 0;
//     const uint32_t currentTime = millis();
//     if (currentTime - lastStateCheck < 1000) return;
//     lastStateCheck = currentTime;
    
//     switch (SystemState::getCurrentState()) {
//         case SystemState::NormalOperation:
//             if (SystemState::getInactiveTime() > systemConfig.sleepTimeout) {
//                 SystemState::setState(SystemState::SleepMode);
//                 displayManager.showMessage("Sleep Mode", "Activated", 2000, 10);
//             }
//             break;
            
//         case SystemState::SleepMode:
//             displayManager.dimDM(true);
//             break;
            
//         case SystemState::ErrorState:
//             digitalWrite(LED_BUILTIN, millis() % 1000 < 500);
//             break;
//     }
// }

// void handleConfigurationPersistence() {
//     static uint32_t lastSaveTime = 0;
//     const uint32_t currentTime = millis();
//     if (currentTime - lastSaveTime > 300000) {
//         for (uint8_t i = 0; i < ButtonsConfig::BUTTONS_SIZE; i++) {
//             systemConfig.buttonActions[i] = buttonHandler.getButtonAction(i + 1);
//         }
//         systemConfig.displayBrightness = displayManager.getCurrentBrightness();
//         systemConfig.sleepTimeout = DisplayConfig::ScreenTimeout;
//         EEPROMManager::save(systemConfig);
//         lastSaveTime = currentTime;
//     }
// }

// void setup() {
//     pinMode(LED_BUILTIN, OUTPUT);
//     Serial.begin(HardwareConfig::SERIAL_BAUD_RATE);
//     Serial.println("System: Starting...");
//     initializePeripherals();
//     SystemState::updateActivity();
//     Serial.println("System: Initialized");
// }

// void loop() {
//     buttonHandler.update();
//     handleButtonEvents();
//     volumeControl.update();
//     displayManager.update();
//     serialProtocol.update();
//     handleSystemStates();
//     handleConfigurationPersistence();
    
//     if (SystemState::isState(SystemState::NormalOperation)) {
//         static uint32_t lastBlink = 0;
//         if (millis() - lastBlink > 1000) {
//             digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
//             lastBlink = millis();
//         }
//     }
    
//     delay(10);
// }

