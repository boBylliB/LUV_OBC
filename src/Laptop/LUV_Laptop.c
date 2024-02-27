/* LUV Laptop Main Loop
*
* Author: Sam Sandelin (2/27/2024)
* Purpose: This file is the jumping off point for the code running on the laptop.
*		   It houses the main running loop and any other core functionality needed to get the rest of the codebase running.
*/

#include "settings.h"
#include "RC_Handler.h"

int main() {
	float nextFrameMotorControl[2] = {};

	bool running = true;

	while (running) {
		// Communicate with BeagleBone
		//float IMUAngles[2];
		//float wheelSpeeds[4];
		//int RCInputData;
		//BeagleBoneComms(nextFrameMotorControl, IMUAngles, wheelSpeeds, &RCInputData);

		// Translate RC Commands
		//float turnDrive[2];
		//RC_Handler(RCInputData, turnDrive);

		// Generate Movement Control
		//MovementController(turnDrive, IMUAngles, wheelSpeeds, nextFrameMotorControl);

		// Log Telemetry
		//TelemetryHandler(IMUAngles, wheelSpeeds, RCInputData, nextFrameMotorControl);
	}

	return 0;
}