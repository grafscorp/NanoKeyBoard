#include "button_handler.hpp"

ButtonHandler::ButtonHandler()
: eventAvailable(false), lastPollTime(0)
{
    for (auto& context : buttonContexts)
    {
        context.stableReadings = 0;
        context.currentState = ButtonState::Idle;
        context.stateChangeTime = 0;
        context.eventPending = false;
    }
    
}

void ButtonHandler::init() {
    for (uint8_t pin : HardwareConfig::BUTTON_PINS)
    {
        pinMode(pin, INPUT_PULLUP);
    }
    memset(rawStates, HIGH, sizeof(rawStates));
    
}

void ButtonHandler::update() {
    const uint32_t currentTime = millis();
    // Throttle polling to configured interval
    if (currentTime - lastPollTime < ButtonsConfig::DebounceIntervalMs)
    {
        return;
    }
    lastPollTime = currentTime;
    readRawState();
    // Process each button independently
    for (uint8_t i = 0; i < ButtonsConfig::BUTTONS_SIZE; ++i)
    {
        processDebounce(i);
        processStateTransition(i);
    }
    
}

bool ButtonHandler::hasEvent() const { return eventAvailable; }

ButtonHandler::ButtonEvent ButtonHandler::getEvent()  
{
     if(!eventAvailable){
        return {0, ButtonState::Idle, 0};
     }

     ButtonEvent  event= currentEvent;
     resetEvent();
     return event;
}

void ButtonHandler::setButtonAction(uint8_t buttonId, uint8_t actionId) 
{
        if (buttonId >= 1 && buttonId <= ButtonsConfig::BUTTONS_SIZE) {
            buttonActions[buttonId - 1] = actionId;
        }
    }

uint8_t ButtonHandler::getButtonAction(uint8_t buttonId) const
{
        if (buttonId >= 1 && buttonId <= ButtonsConfig::BUTTONS_SIZE) {
            return buttonActions[buttonId - 1];
        }
        return 0;
}

void ButtonHandler::executeButtonAction(uint8_t buttonId) {

uint8_t action = getButtonAction(buttonId);
        
        // Временная реализация для демонстрации
        switch (action) {
            case 1: // Toggle playback
                Serial.println(F("[ACTION] Toggle playback"));
                break;
            case 2: // Previous track
                Serial.println(F("[ACTION] Previous track"));
                break;
            case 3: // Next track
                Serial.println(F("[ACTION] Next track"));
                break;
            case 4: // Custom action
                Serial.println(F("[ACTION] Custom action"));
                break;
            }
}

void ButtonHandler::readRawState() {
    for (uint8_t i = 0; i < ButtonsConfig::BUTTONS_SIZE; ++i)
    {
        // Invert logic: pressed = LOW, not pressed = HIGH
        rawStates[i]=digitalRead(HardwareConfig::BUTTON_PINS[i]);
    }
    
}

void ButtonHandler::processDebounce(uint8_t buttonIndex) {
    ButtonContext&context = buttonContexts[buttonIndex];
    const uint8_t currentState = rawStates[buttonIndex];

    // if(currentState == (rawStates[buttonIndex] & 0x01) )
    // {
    //     if(context.stableReadings < ButtonsConfig::DebounceThreshold)
    //     {
    //         ++context.stableReadings;
    //     }
    // }else{
    //     context.stableReadings = 0;
    // }

    if(rawStates[buttonIndex] == (context.stableReadings > 0 ? LOW: HIGH))
    {
        context.stableReadings = 0;
    }else{
        if(context.stableReadings < ButtonsConfig::DebounceThreshold)
        {
            ++context.stableReadings;
        }
    }
}

void ButtonHandler::processStateTransition(uint8_t buttonIndex) {
    ButtonContext& context = buttonContexts[buttonIndex];
    const uint32_t currentTime = millis();

    // Check if state is stable
    const bool isStable = (context.stableReadings >= ButtonsConfig::DebounceThreshold);
    const bool isPressed = (rawStates[buttonIndex] == LOW);
    const uint8_t  newButtonId = static_cast<uint8_t>(buttonIndex+1); 
    switch (context.currentState)
    {
    case ButtonState::Idle:
        if(isStable && isPressed)
        {
            context.currentState = ButtonState::Pressed;
            context.stateChangeTime = currentTime;
            currentEvent = {newButtonId, ButtonState::Pressed,0};
            eventAvailable = true;
        }
        break;
    case ButtonState::Pressed:
        if(!isPressed)
        {
            context.currentState = ButtonState::Released;
            currentEvent= {newButtonId, ButtonState::Released, currentTime-context.stateChangeTime};
            eventAvailable = true;
        }else if(currentTime - context.stateChangeTime > ButtonsConfig::LongPressDuration)
        {
            context.currentState = ButtonState::Held;
            currentEvent ={ newButtonId, ButtonState::Held, currentTime - context.stateChangeTime};
            eventAvailable = true;
        }
        break;
    case ButtonState::Held:
        if(!isPressed)
        {
            context.currentState = ButtonState::Idle;
        }
        break;
    case ButtonState::Released:
        context.currentState = ButtonState::Idle;
        break;
    default:
        break;
    }
}

// uint8_t ButtonHandler::getButtonAction(uint8_t buttonId) const {         
//     if (buttonId >= 1 && buttonId <= ButtonConfig::ButtonCount) {
//             return currentConfig.buttonActions[buttonId - 1];
//         }
//         return 0;}

void ButtonHandler::resetEvent() {
  currentEvent = {0, ButtonState::Idle, 0};
  eventAvailable = false;
}
