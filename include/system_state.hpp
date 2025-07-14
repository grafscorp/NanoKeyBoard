#ifndef SYSTEM_STATE_HPP
#define SYSTEM_STATE_HPP

#include <Arduino.h>

class SystemState {
public:
    enum State : uint8_t {
        NormalOperation,     // Нормальная работа
        ConfigurationMode,   // Режим конфигурации
        SleepMode,           // Режим энергосбережения
        ErrorState           // Аварийное состояние
    };

    // Инициализация состояния
    static void initialize();
    
    // Получить текущее состояние
    static State getCurrentState();
    
    // Установить новое состояние
    static void setState(State newState);
    
    // Обновить время последней активности
    static void updateActivity();
    
    // Проверить, активно ли состояние
    static bool isState(State state);
    
    // Получить время бездействия
    static uint32_t getInactiveTime();

private:
    static volatile State currentState;
    static uint32_t lastActivityTime;
};

#endif