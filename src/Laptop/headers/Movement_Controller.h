#ifndef MOVEMENT_CONTROLLER_H
#define MOVEMENT_CONTROLLER_H

/* Movement Controller
*
* Author: Sam Sandelin (2/27/2024)
* Purpose: Convert a desired speed and turn rate into motor control signals,
*          given IMU angles and wheel speeds
*/

// Converts wheel speeds into turn rate and linear speed
void translateWheelSpeeds(float* wheelSpeeds, float* turnRate, float* linearSpeed);

// Outputs a needed turn output for given desired and actual turn rates
float matchTurn(float desiredTurn, float actualTurn, PIDData* pid);

// Outputs a needed linear output for given desired and actual linear speeds
float matchSpeed(float desiredSpeed, float actualSpeed, PIDData* pid);

// Outputs left-side and right-side motor control signals for a desired speed and turn rate
void MovementController(float* turnDrive, float* IMUAngles, float* wheelSpeeds, float* nextFrameMotorControl, PIDData* speedPID, PIDData* turnPID);

#endif