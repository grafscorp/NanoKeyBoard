#ifndef ENCODER_HPP
#define ENCODER_HPP

#include <Arduino.h>

class Encoder
{

public:
    Encoder() = default;
    explicit  Encoder(const uint8_t pinA, const uint8_t pinB, const uint8_t pulsePerStep = 4  );
    virtual ~Encoder() = default;
    bool init();
    bool init(const uint8_t pinA, const uint8_t pinB, const uint8_t pulsePerStep = 4 );
    void update();
    int8_t getDirection() const;
    int16_t getSteps() const;
    bool isRotated() const;
    void resetSteps();
private:
    uint8_t pinA ;
    uint8_t pinB ;
    uint8_t pulsePerStep=4;
    

    volatile int8_t lastState =0;
    volatile int16_t steps = 0;
    volatile int8_t direction = 0 ;
    volatile bool rotated = false;

    uint8_t readState() const;
};




#endif