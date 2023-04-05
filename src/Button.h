
// Copyright (c) 2023. Leonhard Baschang


#ifndef UNTITLED19_BUTTON_H
#define UNTITLED19_BUTTON_H


class Button {
    uint8_t pin;
    bool pullUp;
    bool activeLow;
    bool currentState;
    bool previousState;

public:
    Button(uint8_t pin, bool pullUp, bool activeLow);
    bool getState();
    bool stateChanged();
};


#endif //UNTITLED19_BUTTON_H
