/* Simplified SBUS Movement
*
* Author: Levi Rushing (3/18/2024)
* Purpose: to convert SBUS control data directly into
*          motor control, for minimum viable movement code
*
* Edited: Sam Sandelin (3/19/2024)
*****/

#include "settings.h"
#include <stdio.h>
#include <stdint.h>

#define MININPUT 172 //defines the input range we could get from channels, came up enough to warrent a define
#define MAXINPUT 1811

//Beginning of subfunctions
float normalizeChannel(float channel) {
    float inputRange = MAXINPUT - MININPUT;
    float midpoint = inputRange/2 + MININPUT;
	float signCheck = channel - midpoint;
	float finalChannel = 2*(signCheck / inputRange);
	return finalChannel;
}

void SBUS2Move(uint16_t* channels, float* motorControl) {
	// We know from the SBUS definition that there are 16 channels
	float driveSpeed = normalizeChannel(channels[0]); //Channel 0 should correspond to a vertical axis input
	float turnRate = normalizeChannel(channels[1]); //Channel 1 should correspond to a horizontal axis input

	// Convert to motor control signals (left and right side)
	motorControl[0] = driveSpeed + turnRate;
	motorControl[1] = driveSpeed - turnRate;
    // Clamp within bounds
    if (motorControl[0] > 1)
        motorControl[0] = 1;
    else if (motorControl[0] < -1)
        motorControl[0] = -1;
    if (motorControl[1] > 1)
        motorControl[1] = 1;
    else if (motorControl[1] < -1)
        motorControl[1] = -1;
    // Check for deadzone
    if (motorControl[0] < DEADZONE && motorControl[0] > -DEADZONE)
        motorControl[0] = 0;
    if (motorControl[1] < DEADZONE && motorControl[1] > -DEADZONE)
        motorControl[1] = 0;
}

void SBUS2Tank(uint16_t* channels, float* motorControl) {
    // We know from the SBUS definition that there are 16 channels
	float rightStick = normalizeChannel(channels[0]); //Channel 0 should correspond to a right stick vertical axis input
	float leftStick = normalizeChannel(channels[2]); //Channel 2 should correspond to a left stick vertical axis input

	// Convert to motor control signals (left and right side)
	motorControl[0] = leftStick;
	motorControl[1] = rightStick;
    // Clamp within bounds
    if (motorControl[0] > 1)
        motorControl[0] = 1;
    else if (motorControl[0] < -1)
        motorControl[0] = -1;
    if (motorControl[1] > 1)
        motorControl[1] = 1;
    else if (motorControl[1] < -1)
        motorControl[1] = -1;
    // Check for deadzone
    if (motorControl[0] < DEADZONE && motorControl[0] > -DEADZONE)
        motorControl[0] = 0;
    if (motorControl[1] < DEADZONE && motorControl[1] > -DEADZONE)
        motorControl[1] = 0;
}
