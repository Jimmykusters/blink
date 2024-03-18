/* Copyright (C) 2020, Frank Vasquez (frank.vasquez@gmail.com) */
#include <stdio.h>
#include <fcntl.h>
#include <fstream>
#include <unistd.h>
#include <pthread.h>

#include <sys/syscall.h>
#include <sys/types.h>

#include <nlohmann/json.hpp>

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
		printf("Command executed successfully. Met Jimmy-image vanuit Ubuntu\n");
	}
	else
	{
		printf("Error executing the command.\n");
	}
}

static void *buttonThread(void *arg)
{
	const std::string filename = "buttonPresses.txt";
	const std::string path = "/tmp/";
	printf("Starting the button thread\n");

	button gpio4(4, DEBOUNCE, NORM_HIGH);
	file_utils::creatFile(path, filename);

	while (1)
	{
		if (gpio4.read())
		{
			WriteCurrentTimeToFile(path, filename);
			printNumberOfPresses(path, filename);
		}

		usleep(1000);
	}
	printf("Stopping the button thread\n");
	return NULL;
}

static void *ledThread(void *arg)
{
	static bool ledState = false;
	GPIO gpio17(17);
	printf("Starting the led thread\n");
	while (1)
	{
		ledState = !ledState;
		gpio17.setValue(ledState);
		sleep(1);
	}
	printf("Stopping the led thread\n");
	return NULL;
}

static void *jsonThread(void *arg)
{
	const std::string filename = "jsonTest.json";
	const std::string path = "/tmp/";
	std::string pathFile = path + filename;

	printf("Starting the JSON thread\n");

	file_utils::creatFile(path, filename);

	nlohmann::json j;

	j["deviceName"] = "raspberry";
	j["mockAPIkey"] = "API:12345";
	j["version"] = 1.2;

	std::cout << j.dump(4) << std::endl;

	file_utils::writeToFile(pathFile, j.dump(4));

	printf("Ending the JSON thread\n");

	return NULL;
}

int main(int argc, char *argv[])
{
	pthread_t btnT, ledT, jsonT;
	printf("Application using threads\n");
	// printUserName(true);

	pthread_create(&btnT, NULL, buttonThread, NULL);
	pthread_create(&ledT, NULL, ledThread, NULL);
	pthread_create(&jsonT, NULL, jsonThread, NULL);

	pthread_join(btnT, NULL);
	pthread_join(ledT, NULL);
	pthread_join(jsonT, NULL);
	printf("App stopped");
	return 0;
}
