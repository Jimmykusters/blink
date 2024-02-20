/* Copyright (C) 2020, Frank Vasquez (frank.vasquez@gmail.com) */
#include <stdio.h>
#include <fcntl.h>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>

#include <chrono>
#include <ctime>

#include "file_utils.h"
#include "button.h"
#include "gpio.h"

static void printCurrentTime(void)
{
	auto current_time = std::chrono::system_clock::now();
	std::time_t time_now = std::chrono::system_clock::to_time_t(current_time);
	std::cout << "Current system time: " << std::ctime(&time_now);
}

static void printNumberOfPresses(const std::string &path, const std::string &filename)
{
	int numberOfButtonPresses = file_utils::countLines(path, filename);
	std::cout << "The button is pressed: " << numberOfButtonPresses << " times " << std::endl;
}


static void WriteCurrentTimeToFile(const std::string &path, const std::string &filename)
{
	std::string pathFile = path + filename;

	auto current_time = std::chrono::system_clock::now();
	std::time_t time_now = std::chrono::system_clock::to_time_t(current_time);
	std::string time_string = std::ctime(&time_now);

	file_utils::appendToFile(pathFile, time_string);
}

static void printUserName(bool printResult)
{
	int result = system("whoami");

	if (!printResult)
	{
		return;
	}

	if (result == 0)
	{
		printf("Command executed successfully. Met Jimmy-image\n");
	}
	else
	{
		printf("Error executing the command.\n");
	}
}

int main(int argc, char *argv[])
{
	const std::string filename = "buttonPresses.txt";
	const std::string path = "/tmp/";

	button gpio4(4, DEBOUNCE, NORM_HIGH);
	GPIO gpio17(17);

	file_utils::creatFile(path, filename);

	printUserName(true);

	while (1)
	{
		if (gpio4.read())
		{
			WriteCurrentTimeToFile(path, filename);
			printNumberOfPresses(path, filename);
			gpio17.setValue(true);
		}
		else{
			gpio17.setValue(false);
		}
		usleep(1000);
	}
	gpio17.setValue(false);
	return 0;
}
