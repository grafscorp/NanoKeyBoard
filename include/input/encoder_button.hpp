#ifndef ENCODER_BUTTON_HPP
#define ENCODER_BUTTON_HPP
#include "input/encoder.hpp"
#include "input/debounced_button.hpp"
class EncoderButton 
{
public:
    explicit EncoderButton(const uint8_t pinButton,const uint8_t pinA=2, const uint8_t pinB=3,const unsigned long debounceButtonDelay = 50 );
    //EncoderButton() = default;
    void init();
    void update();
    bool getButtonIsPressed();
    bool getButtonStateIsChanged();
    bool getEncoderChanged() ;
    int8_t getEncoderSteps() ;
    int8_t getEncoderDirection() ;
private:
    Encoder encoder;
    DebouncedButton button;
    bool buttonChanged = false;

    


};




#endif