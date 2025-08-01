#ifndef DISPLAY_MANAGER_HPP
#define DISPLAY_MANAGER_HPP

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include "config.hpp"

class DisplayManager final
{
public:
    DisplayManager();
    enum DisplayState{
        IDLE,
        SYSTEM,
        MEDIA
    };

    bool init();
    void update();
    
private:

};



#endif

// #include <Adafruit_SSD1306.h>
// #include "config.hpp"


// /**
//  * @class DisplayManager
//  * @brief Advanced OLED display controller with queue system and animations
// */
// class DisplayManager
// {
// public:
//     /**
//      * @enum DisplayState
//      * @brief Current display operation state
//      */
//     enum class DisplayState : uint8_t {
//         Active,         ///< Normal operation
//         Dimmed,         ///< Reduced brightness
//         Sleeping,       ///< Display turned off
//         Animating       ///< Currently playing animation
//     };

//     /**
//      * @struct DisplayMessage
//      * @brief Message container for display queue
//      */
//     struct DisplayMessage {
//         String line1;           ///< Primary text line
//         String line2;           ///< Secondary text line
//         uint16_t duration;      ///< Display duration (ms)
//         bool persistent;        ///< Don't time out
//         uint8_t priority;       ///< Message priority (0-255)
//     };

//     DisplayManager();
    
//     /**
//      * @brief Initialize display hardware
//      * @return true if successful
//      */
//     bool init();

//     /**
//      * @brief Main display processing loop
//      */
//     void update();

//     /**
//      * @brief Show message on display (queued)
//      * @param line1 Primary text
//      * @param line2 Secondary text (optional)
//      * @param duration Display duration (0 = default)
//      * @param priority Message priority (higher = more important)
//      */
//     void showMessage(const String& line1, 
//                      const String& line2 = "", 
//                      uint16_t duration = 0,
//                      uint8_t priority = 10);
    
//     /**
//      * @brief Show predefined system screen
//      * @param screen Screen type from DefaultScreen enum
//      */
//     void showDefaultScreen(DisplayConfig::DefaultScreen screen);

//     /**
//      * @brief Reset display to active state
//      */
//     void wakeUp();

//     /**
//      * @brief Set display brightness
//      * @param brightness 0-255 brightness value
//      */
//     void setBrightness(uint8_t brightness);
//     /**
//      * @brief Get current brightness setting
//      * @return Current brightness value (0-255)
//      */
//     uint8_t getCurrentBrightness() const {
//         return currentBrightness;
//     }
//     /**
//      * @brief Enable or disable display dimming
//      * @param dim true to enable dimming
//      */
//     void dimDM(bool dim);
//     /**
//      * @brief Get current display state
//      */
//     DisplayState getCurrentState() const;
// private:
// uint8_t currentBrightness = 255;
//     /**
//      * @struct MessageQueue
//      * @brief Ring buffer implementation for message queue
//      */
//     struct MessageQueue {
//         DisplayMessage messages[DisplayConfig::MaxMessageQueueSize];
//         uint8_t front = 0;
//         uint8_t rear = 0;
//         uint8_t count = 0;
        
//         bool isEmpty() const;
//         bool isFull() const;
//         bool enqueue(const DisplayMessage& msg);
//         bool dequeue(DisplayMessage& msg);
//         bool peek(DisplayMessage& msg) const;
//     };

//     // Display control
//     Adafruit_SSD1306 display;
//     DisplayState currentState = DisplayState::Sleeping;
//     uint32_t lastActivityTime = 0;
//     uint32_t messageStartTime = 0;
    
//     // Animation control
//     uint8_t scrollOffset = 0;
//     uint32_t lastAnimationTime = 0;
    
//     // Message system
//     MessageQueue messageQueue;
//     DisplayMessage currentMessage;
//     DisplayMessage defaultScreen;
    
//     /**
//      * @brief Process display state machine
//      */
//     void processStateMachine();
    
//     /**
//      * @brief Handle animation updates
//      */
//     void processAnimations();
    
//     /**
//      * @brief Render content to display buffer
//      */
//     void renderFrame();
    
//     /**
//      * @brief Draw two-line message with scrolling
//      */
//     void drawMessageFrame();
    
//     /**
//      * @brief Draw default status screen
//      */
//     void drawDefaultScreen();
    
//     /**
//      * @brief Transition to new display state
//      */
//     void setState(DisplayState newState);
    
//     /**
//      * @brief Apply display power settings
//      */
//     void applyPowerSettings();
// };






// #endif