#include "gpio.h"

GPIO::GPIO(int number) : gpioNumber(number)
{
    exportGPIO();
    setDirection("out");
    std::cout << "GPIO instance initialized " << gpioNumber << " as out" << std::endl;
}

GPIO::GPIO(int number, const std::string &direction) : gpioNumber(number)
{
    exportGPIO();
    setDirection(direction);
    std::cout << "GPIO instance initialized " << gpioNumber << " as " << direction << std::endl;
}

GPIO::~GPIO()
{
    unexportGPIO();
}

void GPIO::setValue(bool value)
{
    // Check if input
    if (direction)
    {
        std::cerr << "Direction is an input " << gpioNumber << std::endl;
    }
    file_utils::writeToFile(getFilePath("value"), (value ? "1" : "0"));
}

bool GPIO::getValue(void)
{
    char value = '0';
    bool result = false;
    // Check if input
    if (!direction)
    {
        // std::cerr << "Direction is an output " << gpioNumber << std::endl;
    }

    std::ifstream valueFile(getFilePath("value"));
    if (valueFile.is_open())
    {
        valueFile >> value;

        if (value == '1')
        {
            result = true;
        }

        valueFile.close();
    }
    else
    {
        std::cerr << "Unable to open value file for GPIO " << gpioNumber << std::endl;
    }
    return result;
}

void GPIO::exportGPIO()
{
    std::string exportString = std::string(GPIO_BASE_PATH) + GPIO_EXPORT;
    file_utils::writeToFile(exportString, std::to_string(gpioNumber));
}

void GPIO::unexportGPIO()
{
    std::string unexportString = std::string(GPIO_BASE_PATH) + GPIO_UNEXPORT;
    file_utils::writeToFile(unexportString, std::to_string(gpioNumber));
}

void GPIO::setDirection(const std::string &direction)
{
    if(direction == "out")
    {
        this->direction = false;
    }
    else
    {
        this->direction =true;
    }

    file_utils::writeToFile(getFilePath("direction"), direction);
}

std::string GPIO::getFilePath(const std::string &fileName)
{
    std::stringstream ss;
    ss << GPIO_BASE_PATH << "gpio" << gpioNumber << "/" << fileName;
    return ss.str();
}