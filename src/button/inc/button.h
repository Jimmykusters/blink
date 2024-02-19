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
    GPIO(int number);

    ~GPIO();

    void setValue(bool value);

private:
    int gpioNumber;

    void exportGPIO();
    void unexportGPIO();
    void setDirection(const std::string& direction);
    std::string getFilePath(const std::string& fileName);
};
