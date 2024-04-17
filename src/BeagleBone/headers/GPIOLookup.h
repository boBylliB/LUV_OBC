#ifndef GPIOLOOKUP_H
#define GPIOLOOKUP_H

// External Function Declarations
int GetGPIONumberFromStr(char* pinStr);
int GetGPIONumber(int bus, int pin);
int GetGPIODirectoryFromStr(char* pinStr, char* GPIODir);
int GetGPIODirectory(int bus, int pin, char* GPIODir);
int GetGPIODirectoryFromNum(int GPIONum, char* GPIODir);
// Internal Helper Functions
void pinStringToIndices(char* pinStr, int* bus, int* pin);
int pinLookup(int bus, int pin);

#endif
