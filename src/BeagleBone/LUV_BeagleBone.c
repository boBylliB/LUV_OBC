/* LUV BeagleBone Main Loop
*
* Author: Sam Sandelin (2/27/2024)
* Purpose: This file is the jumping off point for the code running on the BeagleBone Black.
*		   It houses the main running loop and any other core functionality needed to get the rest of the codebase running.
*/

#include <time.h>
#include <errno.h>
#include "settings.h"
#include "sbus.h"
#include "PWMControl.h"
#include "SimplifiedMovement.h"
#include "BNO08X.h"
#include "GPIOLookup.h"
#include "GPIOControl.h"
#include "Quaternion.h"

int main() {
	// int lastUSCommand = 0;
	float lastMotorCommands[4] = {};
	float motorControl[2] = {};

    struct timespec prevTime;
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC_RAW, &prevTime);

    uint64_t running_ns = 0;

	// Create SBUS controller
	sbus_t* SBUSControl = sbus_new(SBUSUART, SBUSTIMEOUT, SBUS_NONBLOCKING);
    if (SBUSControl == NULL)
        fprintf(stderr, "SBUSControl is null!!!\n");
	uint16_t channels_out[16];
	int missedPacketCount = 0;
    uint64_t sbusTimer = 0;
    uint64_t missedPacketTimer = 0;
    uint64_t sbusPollDelay = SBUSPOLLDELAYMS * 1000000;

    // Setup PWM outputs
    PWMData pwm;
    EnablePWM(&pwm, DEFAULTPERIODNS);

    // Setup IMU
    BNO08x imuControl;
    sh2_SensorValue_t sensorReport;
    Quaternion_t quaternion;
    if (0 != system("config-pin P9_15 gpio"))
        fprintf(stderr, "Failed to set pin P9_15 as gpio!\n");
    if (0 != system("config-pin P9_17 gpio"))
        fprintf(stderr, "Failed to set pin P9_17 as gpio!\n");
    if (0 != system("config-pin P9_19 gpio"))
        fprintf(stderr, "Failed to set pin P9_19 as gpio!\n");
    if (!begin_SPI(&imuControl, GetGPIONumber(9, 17), GetGPIONumber(9, 19), GetGPIONumber(9, 15), 0))
        fprintf(stderr, "Failed to begin SPI!\n");
    if (!enableReport(&imuControl, 0x08, 10000))
        fprintf(stderr, "Failed to enable the Game Rotation Vector report!\n");

    if (0 != system("config-pin P9_12 gpio"))
        fprintf(stderr, "Failed to set pin P9_12 as gpio!\n");
    uint8_t packetLED = -1;
    if (!GetGPIONumber(9, 12))
        fprintf(stderr, "Failed to get GPIO number for P9_12!\n");
    if (!SetGPIO(packetLED, DIRECTION, "out"))
        fprintf(stderr, "Failed to set GPIO direction for P9_12!\n");

    // Setup loop shutoff
    if (0 != system("config-pin P9_41 gpio"))
        fprintf(stderr, "Failed to set pin P9_41 as gpio!\n");
    if (0 != system("config-pin P8_18 gpio"))
        fprintf(stderr, "Failed to set pin P8_18 as gpio!\n");
    setPWMOutput("/sys/class/gpio/gpio20/direction", "out");
    setPWMOutput("/sys/class/gpio/gpio65/direction", "in");
    setPWMOutput("/sys/class/gpio/gpio20/value", "1");
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
        sbusTimer += delta_ns;
        missedPacketTimer += delta_ns;
		if (missedPacketCount > SBUSMAXMISSEDPACKETS) {
            // TODO: change force stop to attempt sbus restart
			fprintf(stderr, "FATAL ERROR: Missed %d packets in a row, greater than the %d allowed!\n", missedPacketCount, SBUSMAXMISSEDPACKETS);
			// Force stop motors until SBUS restarts
			motorControl[0] = 0;
			motorControl[1] = 0;
            // Close and reopen SBUS
            fprintf(stderr, "Restarting SBUS...\n");
            sbus_close(SBUSControl);
	        SBUSControl = sbus_new(SBUSUART, SBUSTIMEOUT, SBUS_NONBLOCKING);
            if (SBUSControl == NULL)
                fprintf(stderr, "SBUSControl is null!!!\n");
	        channels_out[16];
	        missedPacketCount = 0;
            missedPacketTimer = 0;
            sbusTimer = 0;
		}
        if (sbusTimer >= sbusPollDelay /* SBUSPOLLDELAYMS * 1000000 */) {
            // If we've missed more than the acceptable max threshold, increase the poll delay
//            if (missedPacketCount >= SBUSACCEPTABLEMISSEDPACKETS)
//                sbusPollDelay += SBUSPOLLDELAYSHIFTMS * 1000000;
            // If we've missed zero packets in a set time period, decrease the poll delay, as we may be lagging behind
//            if (missedPacketTimer >= SBUSTIMETOMISSEDPACKETMS * 1000000)
//                sbusPollDelay -= SBUSPOLLDELAYSHIFTMS * 1000000;
            sbusTimer = 0;
            int SBUSStatus = sbus_read(SBUSControl, channels_out);
		    if (SBUSStatus < 0) {
			    missedPacketCount++;
                missedPacketTimer = 0;
                // Shut off the received packet LED
                if (!SetGPIO(packetLED, VALUE, "0"))
                    fprintf(stderr, "Failed to set GPIO value for P9_12!\n");
                if (errno != EAGAIN)
			        perror("Error in SBUS: ");
                if (missedPacketCount % 10 == 0)
                    fprintf(stderr, "Missed packet count = %d out of %d\n", missedPacketCount, SBUSMAXMISSEDPACKETS);
			    continue;
		    }
		    else {
			    missedPacketCount = 0;
                // Light the received packet LED
                if (!SetGPIO(packetLED, VALUE, "1"))
                    fprintf(stderr, "Failed to set GPIO value for P9_12!\n");
//                fprintf(stderr, "Channels = ");
                int allZeros = 1;
                for (int idx = 0; idx < 16; idx++) {
//                    fprintf(stderr, "%d, ", channels_out[idx]);
                    if (channels_out[idx] != 0)
                        allZeros = 0;
                }
                fprintf(stderr, "\n");
                // Check for all zeros, and reset SBUS if so
                if (allZeros) {
                    fprintf(stderr, "Channels were all zero, restarting SBUS...\n");
                    sbus_close(SBUSControl);
	                SBUSControl = sbus_new(SBUSUART, SBUSTIMEOUT, SBUS_NONBLOCKING);
                    if (SBUSControl == NULL)
                        fprintf(stderr, "SBUSControl is null!!!\n");
	                channels_out[16];
	                missedPacketCount = 0;
                    sbusTimer = 0;
                }
			    // Convert SBUS into motor control signals
			    SBUS2Tank(channels_out, motorControl); // SBUS2Move or SBUS2Tank, same inputs, different control scheme
		    }
        }
		// Output PWM to motors
        /*motorControl[0] = (time(NULL) - startTime) % 2;
        motorControl[1] = (time(NULL) - startTime + 1) % 2;*/
		OutputPWM(&pwm, motorControl);
        //fprintf(stderr, "Motor Control = %f, %f\n",motorControl[0],motorControl[1]);
//        if (running_ns > 10 * (uint64_t)1000000000)
        FILE* fp = fopen("/sys/class/gpio/gpio65/value","r");
        if (fp == NULL) {
            fprintf(stderr, "FATAL ERROR: Cannot read gpio_46!\n");
            running = 0;
        } else {
            int val = 0;
            fscanf(fp, "%d", &val);
            if (val == 0) {
                fprintf(stderr, "Detected a commanded stop via GPIO_20 and GPIO_61\n");
                running = 0;
            }
            fclose(fp);
        }
        // Read from IMU
        if (GetOrientation(&imuControl, &sensorReport, &quaternion)) {
            double levelAngle = QuaternionToLevelAngle(quaternion);
            fprintf(stderr, "Current level angle: %lf\n", levelAngle);
        }
	}

    DisablePWM(&pwm);
	sbus_close(SBUSControl);
    setPWMOutput("/sys/class/gpio/gpio20/value", "0");

	return 0;
}
