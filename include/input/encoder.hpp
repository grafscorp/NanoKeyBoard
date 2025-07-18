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

private:
    
};




#endif