#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <string.h>
#include "GPIOLookup.h"
#include "settings.h"

/* AutoStartStop
*
* Author: Sam Sandelin (4/9/2024)
* Purpose: Starting & stopping code to control the main BeagleBone run loop
*
* NOTE:    This code needs to be ROBUST as hell while being SIMPLE in what it actually does, as it will be launched on startup and
*		   is the entry point for standard operation
*		   Due to that purpose, it basically needs to be foolproof, and the easiest way to do that is via the KISS principle and a
*		   shit-ton of built-in self tests
*/

/* Each instance of this code will generate a "command" file
*  These command files control the operation of the main run loop
*  If a command file is deleted or modified in such a way to be unreadable, the loop pulling from that file will automatically cease
*  Settings and controls can be sent to the file via echo to modify the functionality of the loop
*/

// Unchangeable settings & constants
typedef enum {
	WAITTIME,
	DEBUGFILE,
	MAINSWITCHA,
	MAINSWITCHB,
	STARTUPLED,
	COMMANDCOUNT
} Command_t;
const char* COMMANDNAMES[] = {
	"waittime",
	"debugfile",
	"mainswitcha",
	"mainswitchb",
	"startupled"
};
typedef enum {
	DIRECTION,
	VALUE
} GPIOFile_t;
// Macros
#define DebugMessage(filename, message) if (!_DebugMessage(filename, message)) return EXIT_FAILURE
#define SetGPIO(GPIONum, target, value) if (!_SetGPIO(GPIONum, target, value)) {DebugMessage(debugFileName, "Failed to set GPIO!"); return EXIT_FAILURE;}
#define _ToLower(c) (c < 'A' || c > 'Z') ? c : (c - ('A' - 'a'))
#define ToLower(string) for (int charIndex = 0; charIndex < BUFFERSIZE && string[charIndex] != '\0'; ++charIndex) _ToLower(string[charIndex])

int _DebugMessage(char* filename, char* message);
int GetCommand(FILE* fp, Command_t* command, char* value);
int _SetGPIO(int GPIONum, GPIOFile_t target, char* value);

int main() {
	// Setup control variables from default settings
	uint64_t standardWait_ms = STANDARDWAITMS;
	uint8_t mainSwitchA = GetGPIONumber(MAINSWSWITCHPINA);
	uint8_t mainSwitchB = GetGPIONumber(MAINSWSWITCHPINB);
	uint8_t startupLED = GetGPIONumber(STARTUPLEDPIN);
	// Get the start time
	struct timespec start;
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC_RAW, &start);
	// Check for an existing control file
	FILE* controlFP = fopen(CONTROLFILENAME,"r");
	// If the file already exists, delete it and wait for a second to allow the process reading that file to stop
	if (fp != NULL) {
		fclose(controlFP);
		if (0 != remove(CONTROLFILENAME)) {
			fprintf(stderr, "FATAL ERROR: Could not delete existing command file!\n");
			return EXIT_FAILURE;
		}
		clock_gettime(CLOCK_MONOTONIC_RAW, &now);
		uint64_t delta_ms = (now.tv_sec - start.tv_sec) * 1000 + (now.tv_nsec - start.tv_nsec) / 1000000;
		while (delta_ms < standardWait_ms) {
			clock_gettime(CLOCK_MONOTONIC_RAW, &now);
			delta_ms = (now.tv_sec - start.tv_sec) * 1000 + (now.tv_nsec - start.tv_nsec) / 1000000;
		}
	}
	// Create a new control file
	controlFP = fopen(CONTROLFILENAME, "w");
	fclose(controlFP);
	// Create a new debug file, appended with the current date and time
	char debugFileName[BUFFERSIZE] = DEBUGFILENAME;
	char currentTime[BUFFERSIZE];
	strftime(currentTime, BUFFERSIZE, "%m-%d-%Y_%H:%M:%S", localtime(time(NULL)));
	sprintf(debugFileName, "%s_%s.%d", DEBUGFILENAME, currentTime, start.tv_nsec / 1000000);
	// Create a new command file, overwriting any existing command files
	FILE* commandFP = fopen(COMMANDFILENAME, "w");
	fclose(commandFP);
	
	Command_t command;
	char commandValue[BUFFERSIZE];
	int exitVal = EXIT_SUCCESS;

	// Set initial GPIO outputs
	SetGPIO(mainSwitchA, DIRECTION, "out");
	SetGPIO(mainSwitchA, VALUE, "1");
	SetGPIO(mainSwitchB, DIRECTION, "in");
	SetGPIO(startupLED, DIRECTION, "out");
	SetGPIO(startupLED, VALUE, "1");

	// Output the startup message to the debug file
	DebugMessage(debugFileName, "Startup code running");
	while (1) {
		// Check that the control file still exists, and stop running if not
		controlFP = fopen(CONTROLFILENAME, "r");
		if (controlFP == NULL)
			break; // We don't return EXIT_FAILURE here because the control file being deleted is the intended exit route
		fclose(controlFP);
		// Check that the command file still exists, and stop running if not
		commandFP = fopen(COMMANDFILENAME, "r");
		if (commandFP == NULL) {
			exitVal = EXIT_FAILURE;
			DebugMessage(debugFileName, "ERROR: Failed to open command file!");
			break;
		}
		// Check for input commands
		while (GetCommand(commandFP, &command, commandValue)) {
			// If commands exist, process them
			switch (command) {
			case WAITTIME:
				int newWaitTime;
				if (sscanf(commandValue, "%d", % newWaitTime) > 0)
					standardWait_ms = newWaitTime;
				else
					DebugMessage(debugFileName, "WARNING: Failed to read wait time from command");
				char message[BUFFERSIZE];
				if (sprintf(message, "New wait time: %d ms", standardWait_ms) > 0)
					DebugMessage(debugFileName, message);
				else
					DebugMessage(debugFileName, "New wait time processed\tWARNING: Failed to append wait time to message");
				break;
			case DEBUGFILE:
				char oldDebugFileName[BUFFERSIZE] = debugFileName;
				debugFileName = commandValue;
				FILE* oldFP = fopen(oldDebugFileName, "r");
				if (oldFP == NULL) {
					DebugMessage(debugFileName, "WARNING: Failed to open old debug file for reading");
					break;
				}
				FILE* newFP = fopen(debugFileName, "a");
				char c;
				while (1) {
					c = fgetc(oldFP);
					if (feof(oldFP))
						break;
					if (fputc(c, newFP) != c)
						break;
				}
				char message[BUFFERSIZE];
				if (sprintf(message, "^^^ Copied from old debug file \"%s\"", oldDebugFileName) > 0)
					DebugMessage(debugFileName, message);
				else
					DebugMessage(debugFileName, "^^^ Copied from old debug file\tWARNING: Failed to append old debug filename to message");
				break;
			}
			case MAINSWITCHA:
			case MAINSWITCHB:
				int newSwitch = GetGPIONumber(commandValue);
				if (newSwitch > -1 && command == MAINSWITCHA) {
					SetGPIO(mainSwitchA, VALUE, "0");
					mainSwitchA = newSwitch;
					SetGPIO(mainSwitchA, DIRECTION, "out");
					SetGPIO(mainSwitchA, VALUE, "1");
				}
				else if (newSwitch > -1) {
					mainSwitchB = newSwitch;
					SetGPIO(mainSwitchA, DIRECTION, "in");
				}
				else
					DebugMessage(debugFileName, "WARNING: Failed to read new switch pin from command");
				char message[BUFFERSIZE];
				if (sprintf(message, "New switch GPIOs: %d - %d", mainSwitchA, mainSwitchB) > 0)
					DebugMessage(debugFileName, message);
				else
					DebugMessage(debugFileName, "New main switch pin processed\tWARNING: Failed to append main switch GPIO numbers to message");
				break;
			case STARTUPLED:
				int newGPIO = GetGPIONumber(commandValue);
				if (newGPIO > -1) {
					SetGPIO(startupLED, VALUE, "0");
					startupLED = newGPIO;
					SetGPIO(startupLED, DIRECTION, "out");
					SetGPIO(startupLED, VALUE, "1");
				}
				else
					DebugMessage(debugFileName, "WARNING: Failed to read new startup LED pin from command");
				char message[BUFFERSIZE];
				if (sprintf(message, "New startup LED GPIO: %d", startupLED) > 0)
					DebugMessage(debugFileName, message);
				else
					DebugMessage(debugFileName, "New startup LED pin processed\tWARNING: Failed to append startup LED GPIO number to message");
				break;
		}
		// Clear the file once processing is done
		fclose(commandFP);
		commandFP = fopen(COMMANDFILENAME, "w");
		fclose(commandFP);
		// Check if main software switch is on, and start the main running loop if so

	}
	
	// Turn off the running LED
	SetGPIO(startupLED, VALUE, "0");
	// Turn off the main switch output
	SetGPIO(mainSwitchA, VALUE, "0");

	// Output a shutdown message to the debug file
	DebugMessage(debugFileName, "Startup code stopped");

	return exitVal;
}

int _DebugMessage(char* filename, char* message) {
	FILE* fp = fopen(filename, "a");
	if (fp == NULL) {
		perror("Error writing to debug file");
		return 0;
	}
	// FORMAT: |h:m:s.ms| message
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC_RAW, &now);
	char currentTime[BUFFERSIZE];
	strftime(currentTime, BUFFERSIZE, "%m-%d-%Y_%H:%M:%S", localtime(time(NULL)));
	fprintf(fp, "|%s.%d| %s\n", currentTime, now.tv_nsec / 1000000, message);

	fclose(fp);
	return 1;
}

int GetCommand(FILE* fp, Command_t* command, char* value) {
	char buffer[BUFFERSIZE];
	// Check for values in the file
	if (fgets(buffer, BUFFERSIZE, fp) == NULL)
		return 0;
	// Read value from the file, only setting the outputs if a match was found (checked and set later)
	char commandName[BUFFERSIZE];
	char tempValue[BUFFERSIZE];
	sscanf(buffer, " %s %[^\n]", commandName, tempValue);
	commandName = ToLower(commandName);
	// Loop through possible commands to search for a match
	int commandSet = 0;
	for (Command_t comIdx = 0; comIdx < COMMANDCOUNT; ++comIdx) {
		if (!strncmp(commandName, COMMANDNAMES[comIdx], BUFFERSIZE)) {
			*command = comIdx;
			strncpy(value, tempValue, BUFFERSIZE);
			commandSet = 1;
			break;
		}
	}
	// Only return 1 if a match was found
	if (commandSet)
		return 1;
	else
		return 0;
}

int _SetGPIO(int GPIONum, GPIOFile_t target, char* value) {
	FILE* fp = NULL;
	char buffer[BUFFERSIZE];
	if (!GetGPIODirectory(GPIONum, buffer)) return 0;
	switch (target) {
	case DIRECTION:
		if (sprintf(buffer, "%s/%s", buffer, "direction") < 1) return 0;
		break;
	case VALUE:
		if (sprintf(buffer, "%s/%s", buffer, "value") < 1) return 0;
		break;
	default:
		return 0;
	}
	fp = fopen(buffer, "a");
	if (fp == NULL) return 0;
	if (fputs(value) == EOF) {
		fclose(fp);
		return 0;
	}
	fclose(fp);
	return 1;
}