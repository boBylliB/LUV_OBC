/* Movement Controller
*
* Author: Sam Sandelin (2/27/2024)
* Purpose: Convert a desired speed and turn rate into motor control signals,
*          given IMU angles and wheel speeds
*/

#include "Movement_Controller.h"
#include "settings.h"
#include "PID.h"

// Converts wheel speeds into turn rate and linear speed
void translateWheelSpeeds(float* wheelSpeeds, float* turnRate, float* linearSpeed) {
	// Average the left and right sides
	float leftSpeed = (wheelSpeeds[0] + wheelSpeeds[1]) / 2;
	float rightSpeed = (wheelSpeeds[2] + wheelSpeeds[3]) / 2;

	// Get the turn rate as the differential speed between the sides
	// In this case, counterclockwise (left) is positive
	&turnRate = rightSpeed - leftSpeed;

	// Get the linear speed as the average between the sides
	&linearSpeed = (leftSpeed + rightSpeed) / 2;
}

// Outputs a needed turn output for given desired and actual turn rates
float matchTurn(float desiredTurn, float actualTurn, PIDData* pid) {
	return getPIDResponse(pid, desiredTurn - actualTurn);
}

// Outputs a needed linear output for given desired and actual linear speeds
float matchSpeed(float desiredSpeed, float actualSpeed, PIDData* pid) {
	return getPIDResponse(pid, desiredSpeed - actualSpeed);
}

// Outputs left-side and right-side motor control signals for a desired speed and turn rate
void MovementController(float* turnDrive, float* IMUAngles, float* wheelSpeeds, float* nextFrameMotorControl, PIDData* speedPID, PIDData* turnPID) {
	// Convert the wheel speed data into relevant speed information
	float turnRate;
	float linearSpeed;
	translateWheelSpeeds(wheelSpeeds, turnRate, linearSpeed);

	// Get desired speed and turn rate
	float desiredTurn = turnDrive[0];
	float desiredSpeed = turnDrive[1];
	if (IMUAngles[0] > MAXPITCH || IMUAngles[0] < -MAXPITCH || IMUAngles[1] > MAXROLL || IMUAngles[1] < -MAXROLL) {
		if (desiredTurn > SAFETYLIMITEDTURN)
			desiredTurn = SAFETYLIMITEDTURN;
		else if (desiredTurn < -SAFETYLIMITEDTURN)
			desiredTurn = -SAFETYLIMITEDTURN;
		if (desiredSpeed > SAFETYLIMITEDSPEED)
			desiredSpeed = SAFETYLIMITEDSPEED;
		else if (desiredSpeed < -SAFETYLIMITEDSPEED)
			desiredSpeed = -SAFETYLIMITEDSPEED;
	}

	// Get PID control outputs for turning and driving
	float turnControl = matchTurn(desiredTurn, turnRate, turnPID);
	float speedControl = matchTurn(desiredSpeed, linearSpeed, speedPID);

	// Convert to motor control outputs
	nextFrameMotorControl[0] = speedControl - turnControl / 2;
	nextFrameMotorControl[1] = speedControl + turnControl / 2;
}