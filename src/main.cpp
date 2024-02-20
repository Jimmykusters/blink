/* Copyright (C) 2020, Frank Vasquez (frank.vasquez@gmail.com) */
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/types.h>

#include <chrono>
#include <ctime>


#include "button.h"
#include "gpio.h"

static void printCurrentTime(void)
{
	auto current_time = std::chrono::system_clock::now();
	std::time_t time_now = std::chrono::system_clock::to_time_t(current_time);
	std::cout << "Current system time: " << std::ctime(&time_now);
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
		printf("Command executed successfully.  Na modify\n");
	}
	else
	{
		printf("Error executing the command.\n");
	}
}

/*
 * Demonstration of using epoll(2) to wait for an interrupt on GPIO.
 *
 * To try this out on a BeagleBone Black, connect a push button switch
 * between P9 15 (gpio1_16) and P9 1 (ground).
 *
 * gpio1_15 is configured as gpio 48, so to make it an input which
 * triggers on a falling edge, write
 *
 * echo 48 > /sys/class/gpio/export
 * echo falling > /sys/class/gpio/gpio48/edge
 *
 * Now, the gpio1_15 pin is normally pulled high, so
 * /sys/class/gpio48/value reads as 1'.
 * Pushing the button takes it low, and value reads as '0'.
 *
 * This program waits for the level to fall from 1 to 0 and
 * prints out a message each time it does so.
 */
int main(int argc, char *argv[])
{
	bool currentState = false;
	int ep;
	int f;
	struct epoll_event ev, events;
	char value[4];
	int ret;
	int n;

	button gpio4(4, DEBOUNCE, NORM_HIGH);

	printUserName(false);

	// ep = epoll_create(1);
	// if (ep == -1)
	// {
	// 	perror("Can't create epoll");
	// 	return 1;
	// }

	// f = open("/sys/class/gpio/gpio4/value", O_RDONLY | O_NONBLOCK);
	// if (f == -1)
	// {
	// 	perror("Can't open gpio4");
	// 	return 1;
	// }

	if (gpio4.read())
	{
		printf("Initial value value=1\n");
	}
	else
	{
		printf("Initial value value=0\n");
	}

	// n = read(f, &value, sizeof(value));
	// if (n > 0)
	// {
	// 	printf("Initial value value=%c\n",
	// 		   value[0]);
	// 	// lseek(f, 0, SEEK_SET);
	// }

	// ev.events = EPOLLPRI;
	// ev.data.fd = f;
	// ret = epoll_ctl(ep, EPOLL_CTL_ADD, f, &ev);
	// if (ret == -1)
	// {
	// 	perror("Can't register target file descriptor with epoll");
	// 	return 1;
	// }

	// Replace 17 with the GPIO number you want to export
	GPIO gpio17(17);
	// Set the GPIO value to HIGH
	gpio17.setValue(true);
	// Wait for a moment (you can replace this with your actual logic)
	sleep(5);
	// Set the GPIO value to LOW
	gpio17.setValue(false);

	while (1)
	{
		if (gpio4.read())
		{
			printf("Is pressed\n");
			printCurrentTime();
			gpio17.setValue(true);
		}
		else{
			gpio17.setValue(false);
		}
		usleep(1000);
		// printf("Waiting\n");
		// ret = epoll_wait(ep, &events, 1, -1);

		// if (ret > 0)
		// {
		// 	n = read(f, &value, sizeof(value));
		// 	printf("Button pressed: read %d bytes, value=%c\n",
		// 		   n, value[0]);
		// 	lseek(f, 0, SEEK_SET);

		// 	printCurrentTime();

		// 	currentState = !currentState;
		// 	gpio17.setValue(currentState);
		// }
	}
	gpio17.setValue(false);
	return 0;
}
