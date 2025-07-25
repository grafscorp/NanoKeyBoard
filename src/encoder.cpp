 #include "input/encoder.hpp"

Encoder* Encoder::instance = nullptr;

void Encoder::handleInterrupt() {
    uint32_t now = micros();
    if (now - lastISR < 1000) return; // Антидребезг 1 мс
    
    lastISR = now;
    uint8_t state = digitalRead(pinA) | (digitalRead(pinB) << 1);
    uint8_t idx = (prevState << 2) | state;
    prevState = state;
    
    lastChange = TRANSITIONS[idx];
    if (lastChange) steps += lastChange;
  }


void Encoder::init()
 {
    instance = this;  // Сохраняем текущий экземпляр
    
    pinMode(pinA, INPUT_PULLUP);
    pinMode(pinB, INPUT_PULLUP);
    prevState = digitalRead(pinA) | (digitalRead(pinB) << 1);
    
    attachInterrupt(digitalPinToInterrupt(pinA), isrWrapperA, CHANGE);
    attachInterrupt(digitalPinToInterrupt(pinB), isrWrapperB, CHANGE);
  }

bool Encoder::changed()
 {
    noInterrupts();
    bool result = (steps != 0);
    interrupts();
    return result;
  }

int8_t Encoder::getSteps()
{
    noInterrupts();
    int32_t result = steps;
    steps = 0;
    interrupts();
    return result;
  }

int8_t Encoder::getDirection()
{
    noInterrupts();
    int8_t result = lastChange;
    lastChange = 0;
    interrupts();
    return result > 0 ? 1 : (result < 0 ? -1 : 0);
  }}
uint8_t Encoder::getModuleSteps()
{
  const int8_t steps = getSteps();
  return steps<0?steps*-1:steps;
    
}