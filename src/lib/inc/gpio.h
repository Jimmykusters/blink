#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define GPIO_BASE_PATH  "/sys/class/gpio/"

#define GPIO_EXPORT     "export"
#define GPIO_UNEXPORT   "unexport"



#define GPIO_DIR_OUT    "out"
#define GPIO_DIR_IN     "in"

// todo edge instellen van een gpio
// todo uitzoeken wat nog meer ingesteld kan worden voor gpio

// GPIO class to handle GPIO operations
class GPIO {
public:
    // Sets the gpio to output
    // @par number pin number of the gpio
    GPIO(int number);
    
    // Allows to set the direction
    // @par number pin number of the gpio
    // @par direction: direction of the gpio "in" or "out"
    GPIO(int number, const std::string& direction);

    ~GPIO();

    void setValue(bool value);
    bool getValue(void);

private:
    int gpioNumber;
    // True = input
    bool direction;

    bool writeToFile(const std::string& path, const std::string& value);

    void exportGPIO();
    void unexportGPIO();
    void setDirection(const std::string& direction);
    std::string getFilePath(const std::string& fileName);
};
