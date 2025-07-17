#ifndef BUTTON_HANDLER_HPP
#define BUTTON_HANDLER_HPP

#include <Arduino.h>
#include "config.hpp"
#include "input/debounced_button.hpp"
#include "interface/I_serial_data_source.hpp"

#define POLLUP_INIT true
#define WITHOUT_PULLUPINIT false


/// @brief Класс для управления нажатием кнопок
class ButtonHandler : public ISerialDataSource
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

    uint8_t getSerialData() override;


private:
/*Данные о кнопках (нажата - 1, не нажата 0)
Пример: 1010 - первая и треться кнопка не нажаты, вторая и четвертая нажаты.
*/
    uint8_t buttonData = 0U;

    DebouncedButton buttons[ButtonsConfig::BUTTONS_COUNT];

//     
};




#endif