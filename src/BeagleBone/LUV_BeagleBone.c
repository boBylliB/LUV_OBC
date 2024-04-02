/* LUV BeagleBone Main Loop
*
* Author: Sam Sandelin (2/27/2024)
* Purpose: This file is the jumping off point for the code running on the BeagleBone Black.
*		   It houses the main running loop and any other core functionality needed to get the rest of the codebase running.
*/

#include <time.h>
#include "settings.h"
#include "sbus.h"
#include "PWMControl.h"
#include "SimplifiedMovement.h"

int main() {
	// int lastUSCommand = 0;
	float lastMotorCommands[4] = {};
	float motorControl[2] = {};

    struct timespec prevTime;
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC_RAW, &prevTime);

    uint64_t running_ns = 0;

	// Create SBUS controller
	sbus_t* SBUSControl = sbus_new(SBUSUART, SBUSTIMEOUT, 0 /*SBUS_NONBLOCKING*/);
    if (SBUSControl == NULL)
        fprintf(stderr, "SBUSControl is null!!!\n");
	uint16_t channels_out[16];
	int missedPacketCount = 0;
    uint64_t sbusTimer = 0;

    // Setup PWM outputs
    PWMData pwm;
    fprintf(stderr, "Enabling PWM\n");
    EnablePWM(&pwm, DEFAULTPERIODNS);
    fprintf(stderr, "Enabled PWM\n");

	int running = 1;

	while (running) {
        clock_gettime(CLOCK_MONOTONIC_RAW, &now);
        uint64_t delta_ns = (now.tv_sec - prevTime.tv_sec) * 1000000000 + (now.tv_nsec - prevTime.tv_nsec);
        clock_gettime(CLOCK_MONOTONIC_RAW, &prevTime);
        running_ns += delta_ns;

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
        fprintf(stderr, "Initializing main loop sequence\n");
        sbusTimer += delta_ns;
		if (missedPacketCount > SBUSMAXMISSEDPACKETS) {
            // TODO: change force stop to attempt sbus restart
			fprintf(stderr, "FATAL ERROR: Missed %d packets in a row, greater than the %d allowed!\n", missedPacketCount, SBUSMAXMISSEDPACKETS);
			running = false;
			// Force stop
			motorControl[0] = 0;
			motorControl[1] = 0;
		}
        if (sbusTimer >= SBUSPOLLDELAYMS * 1000000) {
            sbusTimer = 0;
            fprintf(stderr, "Attempting to read SBUS\n");
            int SBUSStatus = sbus_read(SBUSControl, channels_out);
            fprintf(stderr, "Read SBUS\n");
		    if (SBUSStatus < 0) {
			    missedPacketCount++;
			    fprintf(stderr, "Error in SBUS");
			    continue;
		    }
		    else {
                // TODO: Check for all zeros, reset sbus if so
			    missedPacketCount = 0;
                fprintf(stderr, "Channels = ");
                for (int idx = 0; idx < 16; idx++)
                    fprintf(stderr, "%d, ", channels_out[idx]);
                fprintf(stderr, "\n");
			    // Convert SBUS into motor control signals
			    SBUS2Move(channels_out, motorControl);
		    }
        }
		// Output PWM to motors
        /*motorControl[0] = (time(NULL) - startTime) % 2;
        motorControl[1] = (time(NULL) - startTime + 1) % 2;*/
        fprintf(stderr, "Setting PWM outputs\n");
		OutputPWM(&pwm, motorControl);
        fprintf(stderr, "Motor Control = %f, %f\n",motorControl[0],motorControl[1]);
        if (running_ns > 10 * (uint64_t)1000000000)
            running = 0;
	}

    //DisablePWM(&pwm);
	sbus_close(SBUSControl);

    fprintf(stderr, "Main loop sequence terminated :(\n");

	return 0;
}
