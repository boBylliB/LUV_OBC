#ifndef GPIOCONTROL_H
#define GPIOCONTROL_H

#include <stdint.h>

typedef enum {
	DIRECTION,
	VALUE
} GPIOFile_t;
int SetGPIO(int GPIONum, GPIOFile_t target, char* value);
int GetGPIO(int GPIONum, uint8_t* value);

#endif
