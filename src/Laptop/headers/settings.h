/* LUV Laptop Settings & Defines
*
* Author: Sam Sandelin (2/27/2024)
* Purpose: Houses general settings, constants, and macros, not specific to any particular file in the laptop codebase.
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
// Units: radians/second
#define MAXTURN 2*MAXSPEED/WHEELWIDTH