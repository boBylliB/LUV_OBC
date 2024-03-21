/* LUV BeagleBone Main Loop
*
* Author: Sam Sandelin (2/27/2024)
* Purpose: This file is the jumping off point for the code running on the BeagleBone Black.
*		   It houses the main running loop and any other core functionality needed to get the rest of the codebase running.
*/

#include "settings.h"
#include "sbus.h"
#include "PWMControl.h"
#include "SimplifiedMovement.h"

int main() {
	// int lastUSCommand = 0;
	float lastMotorCommands[4] = {};
	float motorControl[2] = {};

	// Create SBUS controller
	sbus_t* SBUSControl = sbus_new(SBUSUART, SBUSTIMEOUT, SBUS_CONFIG_PINS);
	uint16_t channels_out[16];
	int missedPacketCount = 0;

	int running = 1;

	while (running) {
		// Get data from IMU (and communicate to RF Transceiver in future)
		// int IMUPacket = getIMUData();

		// Get data from ultrasonic sensors (leaving pseudocode for future)
		// float USTimes[12];
		// UltrasonicController(USTimes);

		// Get data from photointerrupt sensors
		// float wheelSpeeds[4];
		// measureWheelSpeed(wheelSpeeds);

		// Get SBUS data from RC receiver
		// int* RCPacket;
		// int RCSuccess = getSBUS(RCPacket);

		// Communicate with laptop
		// PCComms(IMUPacket, wheelSpeeds, RCPacket, RCSuccess, lastMotorCommands);

		// Output PWM to motors
		// MotorController(lastMotorCommands);

		// === TEMPORARY MVP ===
		// Get SBUS data from RC receiver
		int SBUSStatus = sbus_read(SBUSControl, channels_out);
		if (SBUSStatus < 0) {
			missedPacketCount++;
			perror("Error in SBUS: ");
			continue;
		}
		else {
			missedPacketCount = 0;
			// Convert SBUS into motor control signals
			SBUS2Move(channels_out, motorControl);
		}
		if (missedPacketCount > SBUSMAXMISSEDPACKETS) {
			fprintf(stderr, "FATAL ERROR: Missed %d packets in a row, greater than the %d allowed!\n", missedPacketCount, SBUSMAXMISSEDPACKETS);
			running = false;
			// Force stop
			motorControl[0] = 0;
			motorControl[1] = 0;
		}
		// Output PWM to motors
		OutputPWM(motorControl);
	}

	sbus_close(SBUSControl);

	return 0;
}
