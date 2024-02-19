// #include <iostream>

// int main() {
//     std::cout << "Hello, World!" << std::endl;
//     std::cout << "Hello, Gilian!" << std::endl;
//     return 0;
// }

/* Copyright (C) 2020, Frank Vasquez (frank.vasquez@gmail.com) */
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/types.h>

#include <chrono>
#include <ctime>

#include "button.h"
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

	int result = system("whoami");

    if (result == 0) {
        printf("Command executed successfully.\n");
    } else {
        printf("Error executing the command.\n");
    }

	ep = epoll_create(1);
	if (ep == -1) {
		perror("Can't create epoll");
		return 1;
	}

	f = open("/sys/class/gpio/gpio4/value", O_RDONLY | O_NONBLOCK);
	if (f == -1) {
		perror("Can't open gpio4");
		return 1;
	}

	n = read(f, &value, sizeof(value));
	if (n > 0) {
		printf("Initial value value=%c\n",
		       value[0]);
		lseek(f, 0, SEEK_SET);
	}

	ev.events = EPOLLPRI;
	ev.data.fd = f;
	ret = epoll_ctl(ep, EPOLL_CTL_ADD, f, &ev);
	if (ret == -1) {
		perror("Can't register target file descriptor with epoll");
		return 1;
	}

    // Replace 17 with the GPIO number you want to export
    GPIO gpio(17);
    // Set the GPIO value to HIGH
    gpio.setValue(true);
    // Wait for a moment (you can replace this with your actual logic)
    sleep(1);
    // Set the GPIO value to LOW
    gpio.setValue(false);

	while (1) {
		printf("Waiting\n");
		ret = epoll_wait(ep, &events, 1, -1);

		if (ret > 0) {
			n = read(f, &value, sizeof(value));
			printf("Button pressed: read %d bytes, value=%c\n",
			       n, value[0]);
			lseek(f, 0, SEEK_SET);
			
			auto current_time = std::chrono::system_clock::now();
    		std::time_t time_now = std::chrono::system_clock::to_time_t(current_time);
    		std::cout << "Current system time: " << std::ctime(&time_now);

			currentState = !currentState;
			gpio.setValue(currentState);
			
		}
	}
	gpio.setValue(false);
	return 0;
}

