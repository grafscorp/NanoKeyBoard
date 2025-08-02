#include "output/display_manager.hpp"



DisplayManager::DisplayManager(const uint8_t width, const uint8_t heigth, TwoWire *wire, const int8_t rstPin):
display(width, heigth, wire,rstPin)
{
    this->width = width;
    this->heigth = heigth;
}

bool DisplayManager::init(const uint8_t addr)
{

    isInit = display.begin(SSD1306_SWITCHCAPVCC, addr);
    if(isInit)
    {
        
        display.clearDisplay();
        display.display();
        drawIDLE();
    }
    return isInit;
}

void DisplayManager::update()
{
    if(!isInit) return;

    switch (currentState)
    {
    case IDLE:
        /* code */
        break;
    
    default:
        break;
    }
}
void DisplayManager::drawIDLE()
{
    display.drawBitmap(0,0,epd_bitmap_untitled,128,64, SSD1306_WHITE);
    display.display();
}