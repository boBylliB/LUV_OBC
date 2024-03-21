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

#define INPUTRANGE 32768 //defines the input range we could get from channels, came up enough to warrent a define

//Beginning of subfunctions
float normalizeChannel(float channel) {
	float signCheck = channel - INPUTRANGE;
	float finalChannel = signCheck / INPUTRANGE;
	return finalChannel;
}

void SBUS2Move(uint16_t* channels, float* motorControl) {
	// We know from the SBUS definition that there are 16 channels
	float turnRate = normalizeChannel(channels[0]); //Channel 0 should correspond to a horizontal axis input
	float driveSpeed = normalizeChannel(channels[1]); //Channel 1 should correspond to a vertical axis input

	// Convert to motor control signals (left and right side)
	motorControl[0] = driveSpeed - turnRate / 2;
	motorControl[1] = driveSpeed + turnRate / 2;
}
