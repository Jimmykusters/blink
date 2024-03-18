/* Copyright (C) 2020, Frank Vasquez (frank.vasquez@gmail.com) */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <fstream>
#include <unistd.h>
#include <pthread.h>

#include <sys/syscall.h>
#include <sys/types.h>

#include <nlohmann/json.hpp>
#include <curl/curl.h>


#include <chrono>
#include <ctime>

#include "file_utils.h"
#include "button.h"
#include "gpio.h"
#include "ipc_shared_file.h"

#define API_KEY "25964978-bbcc-48ec-a252-96e2d7964461"

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

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t totalSize = size * nmemb;
	size_t numberOfMsg = 0;
	std::string response = std::string(static_cast<char*>(contents), totalSize);

    std::cout << "Received data_: " << response << std::endl;

	nlohmann::json j;
	j = nlohmann::json::parse(response);
	std::cout << "JSON response data_: " << j.dump(4) << std::endl;

	numberOfMsg = j["size"];
	if(numberOfMsg == 0)
	{
		return totalSize;
	}
	else
	{
		for (size_t i = 0; i < numberOfMsg; i++)
		{
			nlohmann::json singleMsg = j["errorMsg"][i];

			if(singleMsg["result"] == "error")
			{
				std::cout << "An error occured" << std::endl;
			}
			else if(singleMsg["result"] == "succes")
			{
				std::cout << "Succes" << std::endl;
			}
			else
			{
				std::cout << "unknown" << std::endl;
			}
		}
	}

    return totalSize;
}

static void *HttpThread(void *arg)
{
	CURL* curl = curl_easy_init();

	printf("Starting the HTTP thread\n");

	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "http://185.132.45.143:5000/data/array");

		// Set the callback function for received data
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

        // Perform the request
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "Error: " << curl_easy_strerror(res) << std::endl;
        }

        // Clean up
        curl_easy_cleanup(curl);
	}

	printf("Ending the HTTP thread\n");

	return NULL;
}

int main(int argc, char *argv[])
{
	pthread_t btnT, ledT, jsonT, httpT;
	printf("Application using threads\n");

	// pthread_create(&btnT, NULL, buttonThread, NULL);
	// pthread_create(&ledT, NULL, ledThread, NULL);
	pthread_create(&jsonT, NULL, jsonThread, NULL);
	pthread_create(&httpT, NULL, HttpThread, NULL);

	// pthread_join(btnT, NULL);
	// pthread_join(ledT, NULL);
	pthread_join(jsonT, NULL);
	pthread_join(httpT, NULL);

	// IPC using a shered file receiving
	IPC_sharedFile_receive();

	printf("App stopped");
	return 0;
}