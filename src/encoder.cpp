#include "input/encoder.hpp"


Encoder::Encoder(const uint8_t pinA, const uint8_t pinB, const uint8_t pulsePerStep) : pinA(pinA), pinB(pinB), pulsePerStep(pulsePerStep)
{

}

bool Encoder::init(const uint8_t pinA, const uint8_t pinB, const uint8_t pulsePerStep)
{
    this->pinA = pinA;
    this->pinB = pinB;
    this->pulsePerStep = pulsePerStep;
    pinMode(pinA, INPUT_PULLUP);
    pinMode(pinB, INPUT_PULLUP);
    lastState = readState();
    return true;
}
bool Encoder::init()
{
    pinMode(pinA, INPUT_PULLUP);
    pinMode(pinB, INPUT_PULLUP);
    lastState = readState();
    return true;
}

void Encoder::update()
{
    uint8_t currentState = readState();
    if(currentState != lastState)
    {
        int8_t dir = 0;

        switch (lastState)
        {
        case 0:
            dir = (currentState == 1)?1:-1;
            break;
        case 1:
            dir = (currentState==3)?1:-1;
            break;
        case 2:
            dir = (currentState==0)?1:-1;
            break;
        case 3:
            dir = (currentState==2)?1:-1;
            break;
        default:
            break;
        }

        if(dir != 0)
        {
            steps += dir;
            direction = dir;
            rotated = true;

            if(abs(steps) % pulsePerStep == 0)
            {
                direction = (steps > 0)?1:-1;
            }
        }
        lastState = currentState;
    }
}

int8_t Encoder::getDirection() const
{
    return direction;
}

int16_t Encoder::getSteps() const
{
    return steps/pulsePerStep;
}

bool Encoder::isRotated() const
{
    return rotated;
}

void Encoder::resetSteps() {
    steps = 0;
    direction = 0;
    rotated = false;
}

uint8_t Encoder::readState() const
{
    return (digitalRead(pinB) <<1 | digitalRead(pinA));
}
