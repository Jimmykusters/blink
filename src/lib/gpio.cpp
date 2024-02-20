#include "gpio.h"

GPIO::GPIO(int number) : gpioNumber(number)
{
    exportGPIO();
    setDirection("out");
}

GPIO::GPIO(int number, const std::string& direction) : gpioNumber(number)
{
    exportGPIO();
    setDirection(direction);
}

GPIO::~GPIO()
{
    unexportGPIO();
}

void GPIO::setValue(bool value)
{
    // Check if input
    if(direction)
    {
        std::cerr << "Direction is an input " << gpioNumber << std::endl;
    }

    std::ofstream valueFile(getFilePath("value"));
    if (valueFile.is_open())
    {
        valueFile << (value ? "1" : "0");
        valueFile.close();
    }
    else
    {
        std::cerr << "Unable to open value file for GPIO " << gpioNumber << std::endl;
    }
}

bool GPIO::getValue(void)
{
    char value = '0';
    bool result = false;
    // Check if input
    if(!direction)
    {
        std::cerr << "Direction is an output " << gpioNumber << std::endl;
    }

    std::ifstream valueFile(getFilePath("value"));
    if (valueFile.is_open())
    {
        valueFile >> value;

        if(value == '1')
        {
            result = true;
        }
        lseek(valueFile, 0, SEEK_SET);

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
    std::ofstream exportFile(exportString);
    if (exportFile.is_open())
    {
        exportFile << gpioNumber;
        exportFile.close();
    }
    else
    {
        std::cerr << "Unable to export GPIO " << gpioNumber << std::endl;
    }
}

void GPIO::unexportGPIO()
{
    std::string unexportString = std::string(GPIO_BASE_PATH) + GPIO_UNEXPORT;
    std::ofstream unexportFile(unexportString);
    if (unexportFile.is_open())
    {
        unexportFile << gpioNumber;
        unexportFile.close();
    }
    else
    {
        std::cerr << "Unable to unexport GPIO " << gpioNumber << std::endl;
    }
}

void GPIO::setDirection(const std::string &direction)
{
    std::ofstream directionFile(getFilePath("direction"));
    if (directionFile.is_open())
    {
        directionFile << direction;
        directionFile.close();
    }
    else
    {
        std::cerr << "Unable to set direction for GPIO " << gpioNumber << std::endl;
    }
}

std::string GPIO::getFilePath(const std::string &fileName)
{
    std::stringstream ss;
    ss << GPIO_BASE_PATH << "gpio" << gpioNumber << "/" << fileName;
    return ss.str();
}