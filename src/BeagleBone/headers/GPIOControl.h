#ifndef GPIOCONTROL_H
#define GPIOCONTROL_H

typedef enum {
	DIRECTION,
	VALUE
} GPIOFile_t;
int SetGPIO(int GPIONum, GPIOFile_t target, char* value);

#endif