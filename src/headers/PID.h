#ifndef PID_H
#define PID_H

/* PID
*
* Author: Sam Sandelin (2/27/2024)
* Purpose: General purpose PID control implementation
*/

typedef struct {
	float kp;
	float ki;
	float kd;

	float lastError;
	float integral;

	float limit;

	float proportionalLimit;
	float integralLimit;
	float derivativeLimit;
} PIDData;

// Used to more easily initialize the PIDData struct
void initializePIDData(PIDData* data, float kp, float ki, float kd, float limit, float proportionalLimit, float integralLimit, float derivativeLimit);

// Get the response of the PID controller to a given error
float getPIDResponse(PIDData* data, float error);

#endif
