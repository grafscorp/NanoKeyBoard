#include "display_manager.hpp"
#include <Arduino.h>

// Font definitions 
const uint8_t* TITLE_FONT = nullptr; // Use default
const uint8_t* BODY_FONT = nullptr;

/**
 * @brief Construct a new DisplayManager object
 * 
 * Initializes display object and internal state
 */
DisplayManager::DisplayManager()
    : display(HardwareConfig::OLED_WIDTH, 
              HardwareConfig::OLED_HEIGHT, 
              &Wire, 
              -1) // Reset pin (-1 = no reset pin)
{
    // Initialize default screen
    defaultScreen = {
        "Media Controller",
        "Ready",
        DisplayConfig::DefaultMessageDuration,
        true,
        1
    };
    
    currentMessage = defaultScreen;
}

/**
 * @brief Initialize display hardware
 * @return true if successful
 */
bool DisplayManager::init() {
    // Initialize I2C communication
    Wire.begin(); 
    Wire.setClock(400000); // Fast mode (400kHz)
    
    // Attempt display initialization
    if(!display.begin(SSD1306_SWITCHCAPVCC, HardwareConfig::OLED_ADDRESS)) {
        return false;
    }
    
    // Configure display settings
    display.setTextColor(SSD1306_WHITE);
    display.clearDisplay();
    display.display();
    
    // Set initial state
    setState(DisplayState::Active);
    lastActivityTime = millis();
    
    return true;
}

/**
 * @brief Main display processing loop
 * 
 * Should be called frequently from main loop
 */
void DisplayManager::update() {
    const uint32_t currentTime = millis();
    
    // Handle state transitions and power management
    processStateMachine();
    
    // Process animations if active
    if(currentState == DisplayState::Animating) {
        if(currentTime - lastAnimationTime >= DisplayConfig::ScrollAnimationDelay) {
            processAnimations();
            lastAnimationTime = currentTime;
        }
    }
    
    // Handle message expiration
    if(!currentMessage.persistent && 
       currentState == DisplayState::Active &&
       currentTime - messageStartTime >= currentMessage.duration) {
        
        if(messageQueue.dequeue(currentMessage)) {
            messageStartTime = currentTime;
        } else {
            currentMessage = defaultScreen;
        }
        
        // Reset animation state
        scrollOffset = 0;
    }
    
    // Render current frame
    renderFrame();
}

/**
 * @brief Process display state machine
 */
void DisplayManager::processStateMachine() {
    const uint32_t inactiveTime = millis() - lastActivityTime;
    
    switch(currentState) {
        case DisplayState::Active:
            if(inactiveTime > DisplayConfig::ScreenTimeout) {
                setState(DisplayState::Dimmed);
            }
            break;
            
        case DisplayState::Dimmed:
            if(inactiveTime > DisplayConfig::ScreenTimeout * 2) {
                setState(DisplayState::Sleeping);
            }
            break;
            
        case DisplayState::Sleeping:
        case DisplayState::Animating:
            // Transitions handled externally
            break;
    }
}

/**
 * @brief Show message on display
 */
void DisplayManager::showMessage(const String& line1, 
                                 const String& line2, 
                                 uint16_t duration,
                                 uint8_t priority) {
    // Ignore low priority messages when queue is full
    if(messageQueue.isFull()) {
        DisplayMessage nextMsg;
        if(messageQueue.peek(nextMsg) && nextMsg.priority > priority) {
            return; // Reject lower priority message
        }
    }
    
    DisplayMessage msg = {
        line1,
        line2,
        duration ? duration : DisplayConfig::DefaultMessageDuration,
        false,
        priority
    };
    
    // Add to queue or display immediately if higher priority
    if(currentMessage.persistent || priority > currentMessage.priority) {
        currentMessage = msg;
        messageStartTime = millis();
        scrollOffset = 0;
        wakeUp();
    } else {
        messageQueue.enqueue(msg);
    }
}

/**
 * @brief Show predefined system screen
 */
void DisplayManager::showDefaultScreen(DisplayConfig::DefaultScreen screen) {
    switch(screen) {
        case DisplayConfig::DefaultScreen::MainStatus:
            defaultScreen = {"Media Controller", "Ready", 0, true, 1};
            break;
            
        case DisplayConfig::DefaultScreen::ButtonAssignment:
            defaultScreen = {"Button Setup", "Press to assign", 0, true, 1};
            break;
            
        case DisplayConfig::DefaultScreen::SystemInfo:
            defaultScreen = {"System Info", "Ver 1.0", 0, true, 1};
            break;
    }
    
    if(currentMessage.persistent) {
        currentMessage = defaultScreen;
        scrollOffset = 0;
    }
}

/**
 * @brief Reset display to active state
 */
void DisplayManager::wakeUp() {
    lastActivityTime = millis();
    if(currentState != DisplayState::Active) {
        setState(DisplayState::Active);
    }
}

void DisplayManager::setBrightness(uint8_t brightness) {
        currentBrightness = brightness;
        display.ssd1306_command(SSD1306_SETCONTRAST);
        display.ssd1306_command(brightness);
}

void DisplayManager::dimDM(bool dim)
{
        if (dim) {
            display.ssd1306_command(SSD1306_SETCONTRAST);
            display.ssd1306_command(DisplayConfig::DimmingBrightness);
        } else {
            setBrightness(currentBrightness);
        }
}

DisplayManager::DisplayState DisplayManager::getCurrentState() const
{
    return currentState;
}

/**
 * @brief Handle animation updates
 */
void DisplayManager::processAnimations() {
    // Simple text scrolling implementation
    const uint16_t textWidth = max(
        currentMessage.line1.length() * 6 * DisplayConfig::BodyFontSize,
        currentMessage.line2.length() * 6 * DisplayConfig::BodyFontSize
    );
    
    if(textWidth > HardwareConfig::OLED_WIDTH) {
        scrollOffset = (scrollOffset + 1) % (textWidth + 20);
    } else {
        scrollOffset = 0;
    }
}

/**
 * @brief Render content to display buffer
 */
void DisplayManager::renderFrame() {
    display.clearDisplay();
    
    switch(currentState) {
        case DisplayState::Active:
        case DisplayState::Animating:
            if(currentMessage.line1.length() || currentMessage.line2.length()) {
                drawMessageFrame();
            } else {
                drawDefaultScreen();
            }
            break;
            
        case DisplayState::Dimmed:
            // Show minimal content
            display.setTextSize(DisplayConfig::BodyFontSize);
            display.setCursor(0, 24);
            display.print("Tap to wake");
            break;
            
        case DisplayState::Sleeping:
            // Blank screen
            break;
    }
    
    display.display();
}

/**
 * @brief Draw message frame with scrolling support
 */
void DisplayManager::drawMessageFrame() {
    const uint8_t lineHeight = 8 * DisplayConfig::BodyFontSize;
    
    // Line 1
    display.setTextSize(DisplayConfig::BodyFontSize);
    display.setCursor(-scrollOffset, 10);
    display.print(currentMessage.line1);
    
    // Line 2 (if present)
    if(currentMessage.line2.length() > 0) {
        display.setCursor(-scrollOffset, 10 + lineHeight + 2);
        display.print(currentMessage.line2);
    }
    
    // Enable animation state if scrolling
    if(scrollOffset > 0) {
        setState(DisplayState::Animating);
    }
}

/**
 * @brief Draw default status screen
 */
void DisplayManager::drawDefaultScreen() {
    // Title
    display.setTextSize(DisplayConfig::TitleFontSize);
    display.setCursor(
        (HardwareConfig::OLED_WIDTH - defaultScreen.line1.length() * 12) / 2, 
        10
    );
    display.print(defaultScreen.line1);
    
    // Status line
    display.setTextSize(DisplayConfig::BodyFontSize);
    display.setCursor(
        (HardwareConfig::OLED_WIDTH - defaultScreen.line2.length() * 6) / 2, 
        40
    );
    display.print(defaultScreen.line2);
}

/**
 * @brief Transition to new display state
 */
void DisplayManager::setState(DisplayState newState) {
    if(currentState == newState) return;
    
    // Exit actions for current state
    switch(currentState) {
        case DisplayState::Sleeping:
            // Wake up display
            display.ssd1306_command(SSD1306_DISPLAYON);
            break;
    }
    
    currentState = newState;
    
    // Entry actions for new state
    switch(newState) {
        case DisplayState::Dimmed:
            display.dim(true);
            break;
            
        case DisplayState::Sleeping:
            display.clearDisplay();
            display.display();
            display.ssd1306_command(SSD1306_DISPLAYOFF);
            break;
            
        case DisplayState::Active:
            display.dim(false);
            break;
    }
    
    applyPowerSettings();
}

/**
 * @brief Apply display power settings
 */
void DisplayManager::applyPowerSettings() {
    switch(currentState) {
        case DisplayState::Dimmed:
            display.ssd1306_command(SSD1306_SETCONTRAST);
            display.ssd1306_command(DisplayConfig::DimmingBrightness);
            break;
            
        case DisplayState::Active:
            display.ssd1306_command(SSD1306_SETCONTRAST);
            display.ssd1306_command(0xFF); // Full brightness
            break;
    }
}

// MessageQueue implementation
bool DisplayManager::MessageQueue::isEmpty() const { 
    return count == 0; 
}

bool DisplayManager::MessageQueue::isFull() const { 
    return count == DisplayConfig::MaxMessageQueueSize; 
}

bool DisplayManager::MessageQueue::enqueue(const DisplayMessage& msg) {
    if(isFull()) return false;
    
    messages[rear] = msg;
    rear = (rear + 1) % DisplayConfig::MaxMessageQueueSize;
    count++;
    return true;
}

bool DisplayManager::MessageQueue::dequeue(DisplayMessage& msg) {
    if(isEmpty()) return false;
    
    msg = messages[front];
    front = (front + 1) % DisplayConfig::MaxMessageQueueSize;
    count--;
    return true;
}

bool DisplayManager::MessageQueue::peek(DisplayMessage& msg) const {
    if(isEmpty()) return false;
    
    msg = messages[front];
    return true;
}