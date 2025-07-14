#include "eeprom_manager.hpp"

void EEPROMManager::initialize() {
        // For platforms that need EEPROM emulation
        #if defined(ESP8266) || defined(ESP32)
        EEPROM.begin(EEPROM_SIZE);
        #endif
}

void EEPROMManager::save(const SystemConfig &config)
{
     // Create a writable copy
        SystemConfig writeConfig = config;
        
        // Calculate checksum before writing
        writeConfig.checksum = calculateChecksum(writeConfig);
        
        // Write to EEPROM
        EEPROM.put(EEPROM_CONFIG_ADDR, writeConfig);
        
        // Commit changes on ESP platforms
        #if defined(ESP8266) || defined(ESP32)
        EEPROM.commit();
        #endif
}

bool EEPROMManager::load(SystemConfig &config)
{
    // Read entire block from EEPROM
    EEPROM.get(EEPROM_CONFIG_ADDR, config);
    
    // Validate configuration
    return validate(config);
}

uint8_t EEPROMManager::calculateChecksum(const SystemConfig &config)
 {
        uint8_t sum = 0xAA;  // Seed value
        const uint8_t* data = reinterpret_cast<const uint8_t*>(&config);
        
        // Calculate checksum over all bytes except the checksum itself
        for (size_t i = 0; i < sizeof(SystemConfig) - sizeof(config.checksum); i++) {
            sum ^= data[i];  // XOR checksum
            sum = (sum << 1) | (sum >> 7);  // Rotate left
        }
        return sum;
    }
bool EEPROMManager::validate(SystemConfig &config)
{
        // Save original checksum
        uint8_t originalChecksum = config.checksum;
        
        // Calculate expected checksum
        uint8_t calculatedChecksum = calculateChecksum(config);
        
        // Restore original checksum for validation
        config.checksum = originalChecksum;
        
        return (originalChecksum == calculatedChecksum);
    }