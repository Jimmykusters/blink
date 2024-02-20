#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define GPIO_BASE_PATH  "/sys/class/gpio/"

#define GPIO_EXPORT     "export"
#define GPIO_UNEXPORT   "unexport"

#define GPIO_DIR_OUT    "out"
#define GPIO_DIR_IN     "in"

// GPIO class to handle GPIO operations
class GPIO {
public:
    // Sets the gpio to output
    GPIO(int number);
    // Allows to set the direction
    GPIO(int number, const std::string& direction);

    ~GPIO();

    void setValue(bool value);
    bool getValue(void);

private:
    int gpioNumber;
    // True = input
    bool direction;

    void exportGPIO();
    void unexportGPIO();
    void setDirection(const std::string& direction);
    std::string getFilePath(const std::string& fileName);
};
