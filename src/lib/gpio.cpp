#include "gpio.h"

GPIO::GPIO(int number) : gpioNumber(number)
{
    exportGPIO();
    setDirection("out");
    direction = false;
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
    writeToFile(getFilePath("value"), (value ? "1" : "0"));
}

bool GPIO::getValue(void)
{
    char value = '0';
    bool result = false;
    // Check if input
    if (!direction)
    {
        std::cerr << "Direction is an output " << gpioNumber << std::endl;
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
    writeToFile(exportString, std::to_string(gpioNumber));
}

void GPIO::unexportGPIO()
{
    std::string unexportString = std::string(GPIO_BASE_PATH) + GPIO_UNEXPORT;
    writeToFile(unexportString, std::to_string(gpioNumber));
}

void GPIO::setDirection(const std::string &direction)
{
    writeToFile(getFilePath("direction"), direction);
}

std::string GPIO::getFilePath(const std::string &fileName)
{
    std::stringstream ss;
    ss << GPIO_BASE_PATH << "gpio" << gpioNumber << "/" << fileName;
    return ss.str();
}

bool GPIO::writeToFile(const std::string &path, const std::string &value)
{
    std::ofstream file(path.c_str());
    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << path << std::endl;
        return false;
    }

    file << value;
    file.close();
    return true;
}