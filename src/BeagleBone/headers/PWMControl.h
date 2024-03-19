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

int convertPowerToPWM(int power_level);
void drivePWM(int dutyCycle, unsigned int reversePin, unsigned int pwmPin);
void OutputPWM(float* power);

#endif