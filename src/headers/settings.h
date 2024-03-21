/* Settings & Defines
*
* Author: Sam Sandelin (2/27/2024)
* Purpose: Houses general settings, constants, and macros, not specific to any particular file in the codebase.
*/

// Including the math library
#include <math.h>

// Math constants used for calculations
#define DEGTORAD M_PI/180.0
#define RADTODEG 180.0/M_PI

// Relevant measurements
// Units: feet
#define WHEELWIDTH 3

// Safety limits and operating bounds
// Units: radians
#define MAXPITCH 20.0*DEGTORAD
#define MAXROLL 20.0*DEGTORAD
// Units: feet/second
#define MAXSPEED 2.9333
#define SAFETYLIMITEDSPEED 1
// Units: radians/second
#define MAXTURN 2*MAXSPEED/WHEELWIDTH
#define SAFETYLIMITEDTURN MAXTURN*SAFETYLIMITEDSPEED/MAXSPEED

// Control and conversion factors
// Units: feet/second per 1 unit of input
#define XINPUTTOTURNRATE MAXTURN
#define YINPUTTOSPEED MAXSPEED

// SBUS settings
#define SBUSUART 1
#define SBUSMAXMISSEDPACKETS 10
// Units: milliseconds
#define SBUSTIMEOUT 50

// GPIO stuff
#define GPIOFolder "/sys/class/gpio"
#define PWMFolder "/sys/class/pwm"

// GPIO Pins for PWM Controller
#define DEFAULTPERIODNS 1000000
// EHRPWM1A, Pin 9.14
#define PWMLEFT pwm3
// EHRPWM2A, Pin 8.19
#define PWMRIGHT pwm5
// MMC1_DAT6
#define REVERSEPWMLEFT P8_3
// MMC1_DAT7
#define REVERSEPWMRIGHT P8_4

// Miscellaneous Defines
#define BUFFERSIZE 256