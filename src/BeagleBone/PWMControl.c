/* PWM Controller
*
* Author: April Paltrineri (3/18/2024)
* Purpose: to intake four motor drive power levels and output
*          to PWM and reversing pins
*
* Edited: Sam Sandelin (3/19/2024)
*****/
#include <stdio.h>
#include <stdlib.h>
#include "settings.h"
#include "PWMControl.h"

void convertPWMPower(PWMData* pwm, float* power) {
    for (int idx = 0; idx < 2; idx++) {
        if (power[0] < 0) {
            pwm->reverse[idx] = 1;
            pwm->power[idx] = power[0] * -1;
        }
        else {
            pwm->reverse[idx] = 0;
            pwm->power[idx] = power[0];
        }
        pwm->power[idx] = pwm->power[idx] > 1 ? 1 : pwm->power[idx];
    }
}

void drivePWM(const PWMData* pwm) {
    setPWMOutput("/sys/class/gpio/gpio38/value", pwm->reverse[0] ? "1" : "0");
    setPWMOutput("/sys/class/gpio/gpio39/value", pwm->reverse[1] ? "1" : "0");
    char buffer[BUFFERSIZE];
    sprintf(buffer, "%d", (int)(pwm->power[0] * pwm->periodNS));
    setPWMOutput("/sys/class/pwm/pwmchip5/pwm1/duty_cycle", buffer);
    sprintf(buffer, "%d", (int)(pwm->power[1] * pwm->periodNS));
    setPWMOutput("/sys/class/pwm/pwmchip5/pwm0/duty_cycle", buffer);
}

int setPWMOutput(const char* filepath, const char* value) {
    FILE* fp = NULL;
    fp = fopen(filepath, "a");
    if (fp != NULL) {
        fputs(value, fp);
        fclose(fp);
        return 1;
    }
    else {
        char buffer[BUFFERSIZE];
        sprintf(buffer, "ERROR in setting %s!", filepath);
        perror(buffer);
        return 0;
    }
}

void OutputPWM(PWMData* pwm, float* power) {
    if (pwm->enabled) {
        convertPWMPower(pwm, power);
        drivePWM(pwm);
    }
}
void EnablePWM(PWMData* pwm, int periodNS) {
    pwm->periodNS = periodNS;
    pwm->enabled = 1;
    // Initialize power levels to 0 for safety
    pwm->power[0] = 0;
    pwm->reverse[0] = 0;
    pwm->power[1] = 0;
    pwm->reverse[1] = 0;
    // Configure PWM pins
    if (0 != system("config-pin P9_16 pwm"))
        fprintf(stderr, "Error configuring P9_16 as pwm!\n");
    if (0 != system("config-pin P9_14 pwm"))
        fprintf(stderr, "Error configuring P9_14 as pwm!\n");
    if (0 != system("config-pin P8_03 gpio"))
        fprintf(stderr, "Error configuring P8_03 as gpio!\n");
    if (0 != system("config-pin P8_04 gpio"))
        fprintf(stderr, "Error configuring P8_04 as gpio!\n");
    // Setup outputs
    setPWMOutput("/sys/class/gpio/gpio38/direction", "out");
    setPWMOutput("/sys/class/gpio/gpio39/direction", "out");
    char buffer[BUFFERSIZE];
    sprintf(buffer, "%d", periodNS);
    setPWMOutput("/sys/class/pwm/pwmchip5/pwm1/period", buffer);
    sprintf(buffer, "%d", 0);
    setPWMOutput("/sys/class/pwm/pwmchip5/pwm1/duty_cycle", buffer);
    sprintf(buffer, "%d", 1);
    setPWMOutput("/sys/class/pwm/pwmchip5/pwm1/enable", buffer);
    sprintf(buffer, "%d", periodNS);
    setPWMOutput("/sys/class/pwm/pwmchip5/pwm0/period", buffer);
    sprintf(buffer, "%d", 0);
    setPWMOutput("/sys/class/pwm/pwmchip5/pwm0/duty_cycle", buffer);
    sprintf(buffer, "%d", 1);
    setPWMOutput("/sys/class/pwm/pwmchip5/pwm0/enable", buffer);
}
void DisablePWM(PWMData* pwm) {
    pwm->enabled = 0;
    setPWMOutput("/sys/class/gpio/gpio38/value", "0");
    setPWMOutput("/sys/class/gpio/gpio39/value", "0");
    char buffer[BUFFERSIZE];
    sprintf(buffer, "%d", 0);
    setPWMOutput("/sys/class/pwm/pwmchip5/pwm1/enable", buffer);
    sprintf(buffer, "%d", 0);
    setPWMOutput("/sys/class/pwm/pwmchip5/pwm0/enable", buffer);
}
