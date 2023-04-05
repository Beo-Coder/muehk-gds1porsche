//
// Created by Leo on 05.04.2023.
//
#include "Arduino.h"
#include "Button.h"


Button::Button(uint8_t pin, bool pullUp, bool activeLow) {
    this->pin = pin;
    this->pullUp = pullUp;
    this->activeLow = activeLow;

    if(pullUp){
        pinMode(pin, INPUT_PULLUP);
    }else{
        pinMode(pin, INPUT);
    }
    previousState = true;
    currentState = false;
}

bool Button::getState() {
    bool state = digitalRead(pin);
    bool value;
    if(state && !activeLow){
        value = true;
    }else if(!state && activeLow){
        value = true;
    }else{
        value = false;
    }
    currentState = state;

    return value;
}

bool Button::stateChanged() {
    currentState = getState();
    if(currentState != previousState){
        previousState = currentState;
        return true;
    }else{
        return false;
    }
}

