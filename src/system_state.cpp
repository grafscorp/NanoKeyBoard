#include "system_state.hpp"

// Инициализация статических переменных
volatile SystemState::State SystemState::currentState = NormalOperation;
uint32_t SystemState::lastActivityTime = 0;

void SystemState::initialize() {
    lastActivityTime = millis();
}

SystemState::State SystemState::getCurrentState() {
    return currentState;
}

void SystemState::setState(State newState) {
    currentState = newState;
}

void SystemState::updateActivity() {
    lastActivityTime = millis();
}

bool SystemState::isState(State state) {
    return currentState == state;
}

uint32_t SystemState::getInactiveTime() {
    return millis() - lastActivityTime;
}