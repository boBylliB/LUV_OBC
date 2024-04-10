#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GPIOLookup.h"
#include "settings.h"

int GetGPIONumber(char* pinStr) {
	int bus = -1;
	int pin = -1;
	pinStringToIndices(pinStr, &bus, &pin);
	return GetGPIONumber(bus, pin);
}
int GetGPIONumber(int bus, int pin) {
	if (pin < 1 || pin > 46 || (bus != 8 && bus != 9))
		return -1;
	return PINLOOKUP[bus - 8][pin-1];
}
int GetGPIODirectory(char* pinStr, char* GPIODir) {
	int bus = -1;
	int pin = -1;
	pinStringToIndices(pinStr, &bus, &pin);
	return GetGPIODirectory(bus, pin, GPIODir);
}
int GetGPIODirectory(int bus, int pin, char* GPIODir) {
	return GetGPIODirectory(GetGPIONumber(bus, pin), GPIODir);
}
int GetGPIODirectory(int GPIONum, char* GPIODir) {
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