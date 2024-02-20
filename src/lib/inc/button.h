#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "gpio.h"

#define DEBOUNCE_TIME_MS 10

typedef enum {
    // Perform no filtering to the button
    RAW,
    // Perform debounce filtering to the button
    DEBOUNCE,
} buttonMode_t;

typedef enum {
    // The button is high while not pressed
    NORM_HIGH,
    // The button is low while not pressed
    NORM_LOW,
} buttonDefaultState_t;

typedef enum {
    IDLE,
    TRANS_TO_PRESSED,
    PRESSED,
    TRANS_TO_RELEASED,
} buttonState_t;

class button : private GPIO
{
private:
    buttonState_t state;
    buttonDefaultState_t defaultState; 
    buttonMode_t mode;
    int debounceCount;

    bool currentstate;
    bool previousstate; 
    /* data */

    bool readDebounce(bool currentButtonState);

public:
    button(int number, buttonMode_t mode, buttonDefaultState_t defaultState);
    ~button();

    bool read(void);
};
