#ifndef PWM_H
#define PWM_H

/* PWM Controller
*
* Author: April Paltrineri (3/18/2024)
* Purpose: to intake four motor drive power levels and output
*          to PWM and reversing pins
*
* Edited: Sam Sandelin (3/19/2024)
*****/

typedef struct {
	float power[2];

	unsigned int periodNS;

	unsigned short int reverse[2];

	unsigned short int enabled;
} PWMData;

/* Internal functions */
void convertPWMPower(PWMData* pwm, float* power);
void drivePWM(const PWMData* pwm);
int setPWMOutput(const char* filepath, const char* value);

/* External functions */
void OutputPWM(PWMData* pwm, float* power);
void EnablePWM(PWMData* pwm, int periodNS);
void DisablePWM(PWMData* pwm);

#endif
