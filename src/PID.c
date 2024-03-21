/* PID
*
* Author: Sam Sandelin (2/27/2024)
* Purpose: General purpose PID control implementation
*/

#include "PID.h"

// Used to more easily initialize the PIDData struct
void initializePIDData(PIDData* data, float kp, float ki, float kd, float limit, float proportionalLimit, float integralLimit, float derivativeLimit) {
	data->kp = kp;
	data->ki = ki;
	data->kd = kd;

	data->limit = limit;

	data->proportionalLimit = proportionalLimit;
	data->integralLimit = integralLimit;
	data->derivativeLimit = derivativeLimit;

	data->lastError = 0;
	data->integral = 0;
}

// Get the response of the PID controller to a given error
float getPIDResponse(PIDData* data, float error) {
	float prop = data->kp * error;
	if (data->proportionalLimit > 0 && prop > data->proportionalLimit)
		prop = data->proportionalLimit;
	else if (data->proportionalLimit > 0 && prop < -data->proportionalLimit)
		prop = -data->proportionalLimit;

	data->integral += error;
	float integ = data->ki * data->integral;
	if (data->integralLimit > 0 && integ > data->integralLimit)
		integ = data->integralLimit;
	else if (data->integralLimit > 0 && integ < -data->integralLimit)
		integ = -data->integralLimit;

	float deriv = data->kd * (error - data->lastError);
	data->lastError = error;
	if (data->derivativeLimit > 0 && deriv > data->derivativeLimit)
		deriv = data->derivativeLimit;
	else if (data->derivativeLimit > 0 && deriv < -data->derivativeLimit)
		deriv = -data->derivativeLimit;

	float output = prop + integ + deriv;
	if (data->limit > 0 && output > data->limit)
		output = data->limit;
	else if (data->limit > 0 && output < -data->limit)
		output = -data->limit;

	return output;
}
