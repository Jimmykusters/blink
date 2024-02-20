#include "button.h"

button::button(int number, buttonMode_t mode, buttonDefaultState_t defaultState) : GPIO(number, "in")
{
    std::cout << "Butten instance initialized " << number << " as in" << std::endl;
    this->state = IDLE;
    this->mode = mode;
    this->defaultState = defaultState;
    this->debounceCount = 0;
}

button::~button()
{

}

bool button::readDebounce(bool currentButtonState) {
    switch (this->state) {
        case IDLE:
            if (currentButtonState) {
                this->state = TRANS_TO_PRESSED;
            }
            break;

        case TRANS_TO_PRESSED:
            if (currentButtonState) {
                this->debounceCount++;
                if (this->debounceCount >= DEBOUNCE_TIME_MS) {
                    this->state = PRESSED;
                }
            } else {
                this->debounceCount = 0;
                this->state = IDLE;
            }
            break;

        case PRESSED:
            if (!currentButtonState) {
                this->state = TRANS_TO_RELEASED;
            }
            break;

        case TRANS_TO_RELEASED:
            if (!currentButtonState) {
                this->debounceCount++;
                if (this->debounceCount >= DEBOUNCE_TIME_MS) {
                    this->state = IDLE;
                    this->debounceCount = 0;
                    return true;  // Button is considered pressed
                }
            } else {
                this->debounceCount = 0;
                this->state = PRESSED;
            }
            break;
    }

    return false;  // Button is not considered pressed
}

bool button::read(void)
{
    bool currentButtonState = getValue();

    if(defaultState = NORM_HIGH)
    {
        // Inverse the button state while its high if not pressed
        currentButtonState = !currentButtonState;
    }

    switch(this->mode)
    {
        case RAW:       return currentButtonState;
        case DEBOUNCE:  return readDebounce(currentButtonState);
    }
}