/* PWM Controller
*
* Author: April Paltrineri (3/18/2024)
* Purpose: to intake four motor drive power levels and output
*          to PWM and reversing pins
*
* Edited: Sam Sandelin (3/19/2024)
*****/
#include <mraa/pwm.h>
#include <stdio.h>
#include "settings.h"
#include "PWM.h"

int convertPowerToPWM(int power_level) {
    return (power_level * 255); // Convert power level (-1 to 1) to duty cycle (-255 to 255)
}
void drivePWM(int dutyCycle, unsigned int reversePin, unsigned int pwmPin) {
    // series of if statements to determine what direction we're going in and initiaties? the pins 
    if (dutyCycle < 0) {
        dutyCycle *= -1; // inverting to output a positive number
        mraa_pwm_init(reversePin, 1); //Reverse motor by setting the reverse pin to high
        mraa_pwm_init(pwmPin); //EHRPWM1A, Pin 14 Left
    }
    else {
        mraa_pwm_init(pwmPin); //EHRPWM1A, Pin 14 Left
    }
}

void OutputPWM(float* power) {
    int dutyCycle[2];
    
    //GPIO Pin stuff
    mraa_pwm_context pwm; 
    mraa_gpio_dir(PWM0, MRAA_GPIO_OUT); //sets the pins to be outputs???
    mraa_gpio_dir(PWM1, MRAA_GPIO_OUT);
    mraa_gpio_dir(PWM2, MRAA_GPIO_OUT);
    mraa_gpio_dir(PWM3, MRAA_GPIO_OUT);
    
    //Gets duty cycle for each power input
    for (int idx = 0; idx < 2; idx++)
        dutyCycle[idx] = convertPowerToPWM(power[idx]);
    
    drivePWM(dutyCycle[0], REVERSEPWM0, PWM0);
    drivePWM(dutyCycle[0], REVERSEPWM1, PWM1);
    drivePWM(dutyCycle[1], REVERSEPWM2, PWM2);
    drivePWM(dutyCycle[1], REVERSEPWM3, PWM3);
}
