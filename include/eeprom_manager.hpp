// #ifndef EEPROM_MANAGER_HPP
// #define EEPROM_MANAGER_HPP

// #include <EEPROM.h>
// #include <Arduino.h>
// #include "config.hpp"

// /**
//  * @class EEPROMManager
//  * @brief Professional EEPROM configuration manager with checksum validation
//  */
// class EEPROMManager
// {
// public:
// /**
//      * @struct SystemConfig
//      * @brief System configuration structure
//      */
//     struct SystemConfig {
//         uint8_t buttonActions[ButtonsConfig::BUTTONS_SIZE]; ///< Button action mappings
//         uint8_t displayBrightness;  ///< OLED brightness (0-255)
//         uint16_t sleepTimeout;      ///< Inactivity timeout (ms)
//         uint8_t checksum;           ///< Configuration checksum
        
//         /// Default constructor
//         SystemConfig() : displayBrightness(255), sleepTimeout(DisplayConfig::ScreenTimeout) {
//             for (uint8_t i = 0; i < ButtonsConfig::BUTTONS_SIZE; i++) {
//                 buttonActions[i] = static_cast<uint8_t>(
//                     static_cast<ButtonsConfig::ButtonDefault>(i + 1)
//                     );
//                 }
//             }
//         };
    
//     /**
//      * @brief Initialize EEPROM subsystem
//      */
//     static void initialize();
//     static void save(const SystemConfig& config);
//     static bool load(SystemConfig& config) ;


// private:
//     // EEPROM configuration address
//     static constexpr int EEPROM_CONFIG_ADDR = 0;
    
//     // EEPROM size (adjust based on your config size)
//     static constexpr size_t EEPROM_SIZE = sizeof(SystemConfig) + 10;
    
//     /**
//      * @brief Calculate configuration checksum
//      * @param config Configuration structure
//      * @return Checksum value
//      */
//     static uint8_t calculateChecksum(const SystemConfig& config);

//     /**
//      * @brief Validate configuration checksum
//      * @param config Configuration to validate
//      * @return true if valid, false otherwise
//      */
//     static bool validate(SystemConfig& config) ;
// };




// #endif