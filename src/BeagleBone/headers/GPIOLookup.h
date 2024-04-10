#ifndef GPIOLOOKUP_H
#define GPIOLOOKUP_H

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

// External Function Declarations
int GetGPIONumber(char* pinStr);
int GetGPIONumber(int bus, int pin);
int GetGPIODirectory(char* pinStr, char* GPIODir);
int GetGPIODirectory(int bus, int pin, char* GPIODir);
int GetGPIODirectory(int GPIONum, char* GPIODir);
// Internal Helper Functions
void pinStringToIndices(char* pinStr, int* bus, int* pin);

#endif