#include "input/debounced_button.hpp"

DebouncedButton::DebouncedButton(const uint8_t pin, const unsigned long debounceDelay) : debounceDelay(debounceDelay)
{
    this->pin = pin;
}

void DebouncedButton::init(const bool inputPULLUP)
{
    pinMode(this->pin, inputPULLUP?INPUT_PULLUP:INPUT);
    lastState = digitalRead(this->pin);
    stableState = lastState;
    lastDebounceTime = millis();
}

void DebouncedButton::init(const uint8_t pin, const unsigned long debounceDelay,const bool inputPULLUP)
{
  this->pin = pin;
  this->debounceDelay = debounceDelay;
  init(inputPULLUP);

}

void DebouncedButton::update()
{
     bool reading = digitalRead(pin);

    if (reading != lastState) {
      lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {
      if (reading != stableState) {
        stableState = reading;
        changed = true;
      } else {
        changed = false;
      }
    }

    lastState = reading;
}

bool DebouncedButton::isReleased()
{
    return this->changed && stableState==HIGH;
}

bool DebouncedButton::isHeld()
{
    return stableState == LOW;
}
bool DebouncedButton::isPressed() 
{
    return this->changed && stableState==LOW;
}