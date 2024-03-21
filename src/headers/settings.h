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

// GPIO Pins for PWM Controller
// Numbers I guess???
//Pin 14
#define PWM0 EHRPWM1A 
//Pin 16 
#define PWM1 EHRPWM1B
//Pin 13
#define PWM2 EHRPWM2B
//Pin 19
#define PWM3 EHRPWM2A
// reverse motor direction 0 
#define REVERSEPWM0 MMC1_DAT6 
//reverse motor direction 1
#define REVERSEPWM1 MMC1_DAT7 
//reverse motor direction 2
#define REVERSEPWM2 MMC1_DAT2 
//reverse motor direction 3
#define REVERSEPWM3 MMC1_DAT3 
