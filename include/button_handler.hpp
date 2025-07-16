#ifndef BUTTON_HANDLER_HPP
#define BUTTON_HANDLER_HPP

#include <Arduino.h>
#include "config.hpp"



/// @brief 
class ButtonHandler
{
public:
    /// @brief 
    ButtonHandler();
    /// @brief Функция инициализации пинов для кнопок. 
    //По умолчанию подключаются со втсроенными подтягивающими резисторами.  
    void init();
    /// @brief 
    void update();
    
};




#endif