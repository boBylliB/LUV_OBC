#ifndef QUATERNION_H
#define QUATERNION_H

/* Quaternion
*
* Author: Sam Sandelin (4/14/2024)
* Purpose: Conversion between quaternions and Euler angles, and some other miscellaneous quaternion math
* 
* Designed to work with q0 quaternions (QW, QX, QY, QZ)
*/

typedef struct {
	double w;
	double x;
	double y;
	double z;
} Quaternion_t;

typedef struct {
	double x;
	double y;
	double z;
} Vector_t;

Vector_t QuaternionToEulerAngles(Quaternion_t quat);
double QuaternionToLevelAngle(Quaternion_t quat);

#endif