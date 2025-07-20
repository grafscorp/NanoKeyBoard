#ifndef ENCODER_BUTTON_HPP
#define ENCODER_BUTTON_HPP
#include "input/encoder.hpp"
#include "input/debounced_button.hpp"

class EncoderButton : public ISerialDataSource
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
    const uint8_t getSerialData() const override;
    const uint8_t getSerialCommand() const override;
    void updateSerialData() override;
private:
    uint8_t data = 0x00;
    Encoder encoder;
    DebouncedButton button;
    bool buttonChanged = false;

    


};




#endif