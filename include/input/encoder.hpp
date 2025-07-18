#ifndef ENCODER_HPP
#define ENCODER_HPP

#include <Arduino.h>

class Encoder {
protected:
  uint8_t pinA, pinB;
  volatile int8_t steps;
  volatile int8_t lastChange;
  volatile uint8_t prevState;
  volatile uint32_t lastISR;  

  // Таблица переходов состояний
  const int8_t TRANSITIONS[16] = {
    0,  // 00 -> 00
    +1, // 00 -> 01
    -1, // 00 -> 10
    0,  // 00 -> 11
    -1, // 01 -> 00
    0,  // 01 -> 01
    0,  // 01 -> 10
    +1, // 01 -> 11
    +1, // 10 -> 00
    0,  // 10 -> 01
    0,  // 10 -> 10
    -1, // 10 -> 11
    0,  // 11 -> 00
    -1, // 11 -> 01
    +1, // 11 -> 10
    0   // 11 -> 11
  };

  // Обработчик прерывания (теперь нестатический)
  void handleInterrupt();
  // Статическая обертка для прерывания
  static void isrWrapperA() { if(instance) instance->handleInterrupt(); }
  static void isrWrapperB() { if(instance) instance->handleInterrupt(); }
  
  // Указатель на экземпляр (для работы с прерываниями)
  static Encoder* instance;

public:
  Encoder(const uint8_t pinA, const uint8_t pinB) : 
    pinA(pinA), pinB(pinB), steps(0), lastChange(0), prevState(0), lastISR(0) {}
  
  void init();

  // Проверка изменения
  bool changed();
  // Получение шагов
  int8_t getSteps();

  // Получение направления
  int8_t getDirection();
};



#endif