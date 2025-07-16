#ifndef BUTTON_HANDLER_HPP
#define BUTTON_HANDLER_HPP

#include <Arduino.h>
#include "config.hpp"


#define POLLUP_INIT true
#define WITHOUT_PULLUPINIT false

/// @brief 
class ButtonHandler
{
public:
    /// @brief 
    ButtonHandler();
    /* @brief Функция инициализации пинов для кнопок. 
    /По умолчанию подключаются со втсроенными подтягивающими резисторами.
    @param POLLUPINIT (по умолчанию) подключть кнопки со встроенным  подтягивающим резистором
    @param WITHOUT_PULLUPINIT подключить кнопки без встроенных подтягивающих резисторов
    */  
    void init(const bool = POLLUP_INIT);

    /// @brief 
    void update();

    void getButtonsStateData();

private:
//Данные: Нажата кнопка(true) или нет (false)
    bool buttonsPressed[ButtonsConfig::BUTTONS_COUNT];
};




#endif