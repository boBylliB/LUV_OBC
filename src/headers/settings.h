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
#define DEADZONE 0.1
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
#define SBUSPOLLDELAYMS 1
#define SBUSPOLLDELAYSHIFTMS 1
#define SBUSUART 4
#define SBUSMAXMISSEDPACKETS 100
#define SBUSACCEPTABLEMISSEDPACKETS 10
#define SBUSTIMETOMISSEDPACKETMS 10
#define SUDO_ASKPASS 1
// Units: milliseconds
#define SBUSTIMEOUT 50

// GPIO stuff
#define GPIOFOLDER "/sys/class/gpio"
#define PWMFOLDER "/sys/class/pwm"

// PWM Controller settings
#define DEFAULTPERIODNS 100000
#define FLIPTIMERMS 1000
#define PWMDISPLAYHEIGHT 23
#define PWMDISPLAYWIDTH 7

// IMU Settings
#define BNO08XINTTIMEOUTMS 500

// Miscellaneous defines
#define BUFFERSIZE 256

// Autostart defines
#define CONTROLFILENAME "startstopcontrol"
#define COMMANDFILENAME "startstopcommands"
#define STANDARDWAITMS 50
#define DEBUGFILENAME "startstopdebug"
#define MAINSWSWITCHPINA "9_41"
#define MAINSWSWITCHPINB "8_18"
#define STARTUPLEDPIN "8_15"