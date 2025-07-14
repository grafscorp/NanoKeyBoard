#ifndef BUTTON_HANDLER_HPP
#define BUTTON_HANDLER_HPP

#include <Arduino.h>
#include "config.hpp"


/**
 * @class ButtonHandler
 * @brief Advanced button state machine with debounce and event detection
 * 
 * Handles mechanical keyboard switches with:
 * - Contact debouncing
 * - Press/release detection
 * - Long-press support
 * - Event-based architecture
 */
class ButtonHandler
{
public:

    /**
    @enum ButtonState
    @brief Represents physical button states
     */
    enum class ButtonState{
        Idle,       // Button not pressed
        Pressed,    // Button pressed (debounced)
        Held,       // Button held for extended period
        Released    // Button released
    };

    /**
     * @struct ButtonEvent
     * @brief Button event descriptor
     */
    struct ButtonEvent
    {
        uint8_t buttonID;
        ButtonState state;
        uint32_t duration;
    };

    ButtonHandler();
    /**
     * @brief Initialize button pins and internal state
     */
    void init();
    /**
     * @brief Main processing loop - must be called frequently
     */
    void update();
    /**
     * @brief Check if events are pending
     * @return true if events available
     */
    bool hasEvent() const;
    /**
     * @brief Retrieve next button event
     * @return ButtonEvent structure
     */
    ButtonEvent getEvent();
    // /**
    //  * @brief Get action ID for specified button
    //  * @param buttonId Physical button ID (1-based)
    //  * @return Action ID
    //  */
    // uint8_t getButtonAction(uint8_t buttonId) const;

private:
    /**
     * @struct ButtonContext
     * @brief Per-button tracking context
     */
    struct ButtonContext
    {
        uint8_t stableReadings;     // Consecutive stable readings
        ButtonState currentState;   // Current FSM state
        uint32_t stateChangeTime;   // Last state change timestamp
        bool eventPending;
    };
// Internal state
    ButtonContext buttonContexts[ButtonsConfig::BUTTONS_SIZE];
    ButtonEvent currentEvent;
    bool eventAvailable ;
    /**
     * @brief Read raw button states into internal buffer
     */
    void readRawState();
    /**
     * @brief Process debounce logic for specific button
     * @param buttonIndex Button index (0-based)
    */
    void processDebounce(uint8_t buttonIndex);
    /**
     * @brief Handle state transitions for button
     * @param buttonIndex Button index (0-based)
     */
    void processStateTransition(uint8_t buttonIndex);



    /**
     * @brief Reset current event buffer
     */
    void resetEvent();
    // Raw pin states storage
    uint8_t rawStates[ButtonsConfig::BUTTONS_SIZE];
    uint32_t lastPollTime;

    friend class SerialProtocol;
    
};




#endif