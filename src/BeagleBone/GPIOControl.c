#include "GPIOControl.h"
#include "GPIOLookup.h"
#include "settings.h"
#include <stdio.h>
#include <stdint.h>

int SetGPIO(int GPIONum, GPIOFile_t target, char* value) {
	FILE* fp = NULL;
	char buffer[BUFFERSIZE] = {};
	if (!GetGPIODirectoryFromNum(GPIONum, buffer)) return 0;
	switch (target) {
	case DIRECTION:
		if (snprintf(buffer, BUFFERSIZE, "%s/%s", buffer, "direction") < 1) return 0;
		break;
	case VALUE:
		if (snprintf(buffer, BUFFERSIZE, "%s/%s", buffer, "value") < 1) return 0;
		break;
	default:
		return 0;
	}
	fp = fopen(buffer, "a");
	if (fp == NULL) return 0;
	if (fputs(value, fp) == EOF) {
		fclose(fp);
		return 0;
	}
	fclose(fp);
	return 1;
}
int GetGPIO(int GPIONum, uint8_t* value) {
	FILE* fp = NULL;
	char buffer[BUFFERSIZE] = {};
	if (!GetGPIODirectoryFromNum(GPIONum, buffer)) return 0;
	if (snprintf(buffer, BUFFERSIZE, "%s/%s", buffer, "value") < 1) return 0;
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
