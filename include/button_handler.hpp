#ifndef BUTTON_HANDLER_HPP
#define BUTTON_HANDLER_HPP

#include <Arduino.h>
#include "config.hpp"
class ButtonHandler
{
public:
    enum class ButtonState{
        Idle,       // Button not pressed
        Pressed,    // Button pressed (debounced)
        Held,       // Button held for extended period
        Released    // Button released
    };
    struct ButtonEvent
    {
        uint8_t buttonID;
        ButtonState state;
        uint32_t duration;
    };
    ButtonHandler();
    void init();
    void update();
    bool hasEvent() const;
    ButtonEvent getEvent();
private:
    struct ButtonContext
    {
        uint8_t stableReadings;     // Consecutive stable readings
        ButtonState currentState;   // Current FSM state
        uint32_t stateChangeTime;   // Last state change timestamp
        bool eventPending;
    };
    ButtonContext buttonContexts[ButtonsConfig::BUTTONS_SIZE];
    ButtonEvent currentEvent;
    bool eventAvailable ;
    void readRawState();
    void processDebounce(uint8_t buttonIndex);
    void processStateTransition(uint8_t buttonIndex);
    void resetEvent();
    uint8_t rawStates[ButtonsConfig::BUTTONS_SIZE];
    uint32_t lastPollTime;
};




#endif