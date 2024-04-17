#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GPIOLookup.h"
#include "settings.h"

int GetGPIONumberFromStr(char* pinStr) {
	int bus = -1;
	int pin = -1;
	pinStringToIndices(pinStr, &bus, &pin);
	return GetGPIONumber(bus, pin);
}
int GetGPIONumber(int bus, int pin) {
	if (pin < 1 || pin > 46 || (bus != 8 && bus != 9))
		return -1;
	return pinLookup(bus - 8)(pin-1);
}
int GetGPIODirectoryFromStr(char* pinStr, char* GPIODir) {
	int bus = -1;
	int pin = -1;
	pinStringToIndices(pinStr, &bus, &pin);
	return GetGPIODirectory(bus, pin, GPIODir);
}
int GetGPIODirectory(int bus, int pin, char* GPIODir) {
	return GetGPIODirectoryFromNum(GetGPIONumber(bus, pin), GPIODir);
}
int GetGPIODirectoryFromNum(int GPIONum, char* GPIODir) {
	if (GPIONum < 0)
		return 0;
	snprintf(GPIODir, BUFFERSIZE, "%s/%s%d", GPIOFOLDER, "gpio", GPIONum);
	return 1;
}

void pinStringToIndices(char* pinStr, int* bus, int* pin) {
	int tempBus = -1;
	int tempPin = -1;
	if (sscanf(pinStr, "%*[^0-9]%d%*[._]%d", &tempBus, &tempPin) < 2 || tempPin > 46 || tempPin < 1 || (tempBus != 8 && tempBus != 9))
		return;
	else {
		*bus = tempBus;
		*pin = tempPin;
	}
}
int pinLookup(int bus, int pin) {
	const int PINLOOKUP[2][46] = {
	{-1, -1, // P8
	 38, 39,
	 34, 35,
	 66, 67,
	 69, 68,
	 45, 44,
	 23, 26,
	 47, 46,
	 27, 65,
	 22, 63,
	 62, 37,
	 36, 33,
	 32, 61,
	 86, 88,
	 87, 89,
	 10, 11,
	 9, 81,
	 8, 80,
	 78, 79,
	 76, 77,
	 74, 75,
	 72, 73,
	 70, 71},
	{-1, -1, // P9
	 -1, -1,
	 -1, -1,
	 -1, -1,
	 -1, -1,
	 30, 60,
	 31, 40,
	 48, 51,
	 4, 5,
	 13, 12,
	 3, 2,
	 49, 15,
	 117, 14,
	 125, 123,
	 111, 112,
	 110, -1,
	 -1, -1,
	 -1, -1,
	 -1, -1,
	 -1, -1,
	 20, 7,
	 -1, -1,
	 -1, -1}
	};

	return PINLOOKUP[bus][pin];
}