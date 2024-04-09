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
#include <stdint.h>
#include <time.h>
#include "settings.h"
#include "PWMControl.h"

void convertPWMPower(PWMData* pwm, float* power) {
    struct timespec currentTime;
    uint8_t canFlip;
    uint64_t delta_ms;
    uint8_t desiredFlip;
    for (int idx = 0; idx < 2; idx++) {
        clock_gettime(CLOCK_MONOTONIC_RAW, &currentTime);
        delta_ms = (currentTime.tv_sec * 1000 + currentTime.tv_nsec / 1000000) - pwm->flipTimers[idx];
        canFlip = (delta_ms > FLIPTIMERMS);
        desiredFlip = 0;
        // Check for direction
        if (power[idx] < 0) {
            pwm->power[idx] = power[idx] * -1;
            if (pwm->reverse[idx] == 0 && canFlip) {
                desiredFlip = 1;
                pwm->flipTimers[idx] = currentTime.tv_sec * 1000 + currentTime.tv_nsec / 1000000;
            }
        }
        else {
            pwm->power[idx] = power[idx];
            if (pwm->reverse[idx] == 1 && canFlip) {
                desiredFlip = 1;
                pwm->flipTimers[idx] = currentTime.tv_sec * 1000 + currentTime.tv_nsec / 1000000;
            }
        }
        // Set pwm power
        pwm->power[idx] = pwm->power[idx] > 1 ? 1 : pwm->power[idx];
        // Flip reverse command if needed
        if (desiredFlip)
            pwm->reverse[idx] = 1 - pwm->reverse[idx];
        // Scale pwm power to not be linear
        pwm->power[idx] = pwm->power[idx] * pwm->power[idx];
    }
}

void drivePWM(const PWMData* pwm) {
//    fprintf(stderr, "%d, %d\n", (int)(pwm->power[0] * pwm->periodNS), (int)(pwm->power[1] * pwm->periodNS));
    setPWMOutput("/sys/class/gpio/gpio45/value", pwm->reverse[0] ? "1" : "0");
    setPWMOutput("/sys/class/gpio/gpio44/value", pwm->reverse[1] ? "0" : "1");
    char buffer[BUFFERSIZE];
    sprintf(buffer, "%d", (int)(pwm->power[0] * pwm->periodNS));
    setPWMOutput("/sys/class/pwm/pwmchip5/pwm0/duty_cycle", buffer);
    sprintf(buffer, "%d", (int)(pwm->power[1] * pwm->periodNS));
    setPWMOutput("/sys/class/pwm/pwmchip7/pwm0/duty_cycle", buffer);
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
    pwm->reverse[0] = 0;
    pwm->power[0] = 0;
    pwm->reverse[1] = 0;
    pwm->power[1] = 0;
    pwm->flipTimers[0] = FLIPTIMERMS;
    pwm->flipTimers[1] = FLIPTIMERMS;
    // Configure PWM pins
    if (0 != system("config-pin P9_16 pwm"))
        fprintf(stderr, "Error configuring P9_16 as pwm!\n");
    if (0 != system("config-pin P9_14 pwm"))
        fprintf(stderr, "Error configuring P9_14 as pwm!\n");
    if (0 != system("config-pin P8_13 pwm"))
        fprintf(stderr, "Error configuring P8_13 as pwm!\n");
    if (0 != system("config-pin P8_19 pwm"))
        fprintf(stderr, "Error configuring P8_19 as pwm!\n");
    if (0 != system("config-pin P8_11 gpio"))
        fprintf(stderr, "Error configuring P8_11 as gpio!\n");
    if (0 != system("config-pin P8_12 gpio"))
        fprintf(stderr, "Error configuring P8_12 as gpio!\n");
    // Setup outputs
    setPWMOutput("/sys/class/gpio/gpio45/direction", "out");
    setPWMOutput("/sys/class/gpio/gpio44/direction", "out");
    char buffer[BUFFERSIZE];
    sprintf(buffer, "%d", periodNS);
    setPWMOutput("/sys/class/pwm/pwmchip7/pwm0/period", buffer);
    sprintf(buffer, "%d", 0);
    setPWMOutput("/sys/class/pwm/pwmchip7/pwm0/duty_cycle", buffer);
    sprintf(buffer, "%d", 1);
    setPWMOutput("/sys/class/pwm/pwmchip7/pwm0/enable", buffer);
    sprintf(buffer, "%d", periodNS);
    setPWMOutput("/sys/class/pwm/pwmchip5/pwm0/period", buffer);
    sprintf(buffer, "%d", 0);
    setPWMOutput("/sys/class/pwm/pwmchip5/pwm0/duty_cycle", buffer);
    sprintf(buffer, "%d", 1);
    setPWMOutput("/sys/class/pwm/pwmchip5/pwm0/enable", buffer);
}
void DisablePWM(PWMData* pwm) {
    pwm->enabled = 0;
    setPWMOutput("/sys/class/gpio/gpio45/value", "0");
    setPWMOutput("/sys/class/gpio/gpio44/value", "0");
    char buffer[BUFFERSIZE];
    sprintf(buffer, "%d", 0);
    setPWMOutput("/sys/class/pwm/pwmchip7/pwm0/enable", buffer);
    sprintf(buffer, "%d", 0);
    setPWMOutput("/sys/class/pwm/pwmchip5/pwm0/enable", buffer);
}
