#include "GPIOControl.h"
#include "GPIOLookup.h"
#include "settings.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

int SetGPIO(int GPIONum, GPIOFile_t target, char* value) {
	FILE* fp = NULL;
	char buffer[BUFFERSIZE] = {};
	if (!GetGPIODirectoryFromNum(GPIONum, buffer)) { fprintf(stderr, "Failed to get GPIO directory!\n"); return 0; }
    //fprintf(stderr, "GPIO Directory output: %s\n", buffer);
	switch (target) {
	case DIRECTION:
        strcat(buffer, "/direction");
		//if (snprintf(buffer, BUFFERSIZE, "%s/%s", buffer, "direction") < 1) { fprintf(stderr, "Failed to write direction to buffer!\n"); return 0; }
		break;
	case VALUE:
        strcat(buffer, "/value");
		//if (snprintf(buffer, BUFFERSIZE, "%s/%s", buffer, "value") < 1) { fprintf(stderr, "Failed to write value to buffer!\n"); return 0; }
		break;
	default:
        fprintf(stderr, "Unknown target %d\n", target);
		return 0;
	}
	fp = fopen(buffer, "a");
	if (fp == NULL) {fprintf(stderr, "fp is null, could not open file %s\n", buffer); return 0;}
	if (fputs(value, fp) == EOF) {
		fclose(fp);
        fprintf(stderr, "Failed to write to GPIO file!\n");
		return 0;
	}
    //fprintf(stderr, "SetGPIO Success\n");
	fclose(fp);
	return 1;
}
int GetGPIO(int GPIONum, uint8_t* value) {
	FILE* fp = NULL;
	char buffer[BUFFERSIZE] = {};
	if (!GetGPIODirectoryFromNum(GPIONum, buffer)) return 0;
	//if (snprintf(buffer, BUFFERSIZE, "%s/%s", buffer, "value") < 1) return 0;
    strcat(buffer, "/value");
	fp = fopen(buffer, "r");
	if (fp == NULL) return 0;
	char c = fgetc(fp);
	if (c == EOF) {
		fclose(fp);
		return 0;
	}
	*value = (c == '1') ? 1 : 0;
	fclose(fp);
	return 1;
}
