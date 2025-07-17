#include "button_handler.hpp"

ButtonHandler::ButtonHandler() {
    //Ставим данные кнопок в положенние Released - не нажаты
    for (auto& buttonPressed : buttonsPressed)
    {
        buttonPressed = false;
    }
     


}


void ButtonHandler::init(const bool initPullUp)
{
    for (auto buttonPin : HardwareConfig::BUTTON_PINS)
    {
        pinMode(buttonPin, initPullUp?INPUT_PULLUP:INPUT );
    }
    
}

void ButtonHandler::update()
{
    // for (uint8_t i = 0; i < ButtonsConfig::BUTTONS_COUNT; i++)
    // {
    //     //Записываем состояние кнопки, так как по умолчанию стоит подтягивающий резистор, значение true с кнопки означает что она не нажата
    //     buttonsPressed[i] = !(static_cast<bool>(digitalRead(HardwareConfig::BUTTON_PINS[i])));
    //     0b00;
    // }
    this->buttonData = 0U;
    for (uint8_t i = ButtonsConfig::BUTTONS_COUNT-1; i >=0; --i)
    {
        buttonData <<= 1U;
        buttonData |= (digitalRead(HardwareConfig::BUTTON_PINS[i]) & 1U);
    }
    
    
}

uint8_t ButtonHandler::getButtonsStateData() {
    return buttonData;
}
