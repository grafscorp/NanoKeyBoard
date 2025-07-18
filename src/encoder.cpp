 #include "input/encoder.hpp"


// // Encoder::Encoder(const uint8_t pinA, const uint8_t pinB, const uint8_t pulsePerStep) : pinA(pinA), pinB(pinB), pulsePerStep(pulsePerStep)
// // {

// // }

// // bool Encoder::init(const uint8_t pinA, const uint8_t pinB, const uint8_t pulsePerStep)
// // {
// //     this->pinA = pinA;
// //     this->pinB = pinB;
// //     this->pulsePerStep = pulsePerStep;
// //     pinMode(pinA, INPUT_PULLUP);
// //     pinMode(pinB, INPUT_PULLUP);
// //     lastState = readState();
// //     return true;
// // }
// // bool Encoder::init()
// // {
// //     pinMode(pinA, INPUT_PULLUP);
// //     pinMode(pinB, INPUT_PULLUP);
// //     lastState = readState();
// //     return true;
// // }

// // void Encoder::update()
// // {
// //     uint8_t currentState = readState();
// //     if(currentState != lastState)
// //     {
// //         int8_t dir = 0;

// //         switch (lastState)
// //         {
// //         case 0:
// //             dir = (currentState == 1)?1:-1;
// //             break;
// //         case 1:
// //             dir = (currentState==3)?1:-1;
// //             break;
// //         case 2:
// //             dir = (currentState==0)?1:-1;
// //             break;
// //         case 3:
// //             dir = (currentState==2)?1:-1;
// //             break;
// //         default:
// //             break;
// //         }

// //         if(dir != 0)
// //         {
// //             steps += dir;
// //             direction = dir;
// //             rotated = true;

// //             if(abs(steps) % pulsePerStep == 0)
// //             {
// //                 direction = (steps > 0)?1:-1;
// //             }
// //         }
// //         lastState = currentState;
// //     }
// // }

// // int8_t Encoder::getDirection() const
// // {
// //     return direction;
// // }

// // int16_t Encoder::getSteps() const
// // {
// //     return steps/pulsePerStep;
// // }

// // bool Encoder::isRotated() const
// // {
// //     return rotated;
// // }

// // void Encoder::resetSteps() {
// //     steps = 0;
// //     direction = 0;
// //     rotated = false;
// // }

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


// // uint8_t Encoder::readState() const
// // {
// //     return (digitalRead(pinB) <<1 | digitalRead(pinA));
// // }
// //  steps;     // Накопленные шаги
// //   volatile int8_t lastChange; // Последнее изменение (-1, 0, 1)
// //   volatile uint8_t prevState; // Предыдущее состояние
// //   volatile uint32_t lastISR;
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

int32_t Encoder::getSteps()
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
  }