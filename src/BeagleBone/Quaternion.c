#include "Quaternion.h"
#include <math.h>

Vector_t QuaternionToEulerAngles(Quaternion_t quat) {
	Vector_t euler;
	// Using an axis definition where yaw is about a vertical z axis, pitch is about a right-facing x axis,
	// and roll is about a forward y axis
	// This means that yaw, pitch, roll becomes a 312 sequence
	// DCM:
	// [	cos(R)*cos(Y) + sin(P)*sin(R)*sin(Y),	cos(Y)*sin(P)*sin(R) - cos(R)*sin(Y),	cos(P)*sin(R)	]
	// [	cos(P)*sin(Y),							cos(P)*cos(Y),							-sin(P)			]
	// [	cos(R)*sin(P)*sin(Y) - cos(Y)*sin(R),	sin(R)*sin(Y) + cos(R)*cos(Y)*sin(P),	cos(P)*cos(R)	]
	euler.x = asin(-2 * (quat.y * quat.z - quat.w * quat.x));
	euler.y = asin(2 * (quat.w * quat.y + quat.x * quat.z) / cos(euler.x));
	euler.z = asin(2 * (quat.x * quat.z - quat.w * quat.y) / cos(euler.x));

	return euler;
}
double QuaternionToLevelAngle(Quaternion_t quat) {
	// The angle from level is equivalent to the angle from a perfectly vertical axis
	// We can consider a vector being rotated by the quaternion, starting from vertical, and then calculate the angle between it and vertical
	// By using unit vectors this simplies the math greatly
	// In fact, we can pull this almost directly from the bottom right value of the DCM, which can be obtained directly from the quaternion
	// All we have to do is take the inverse cosine of that value to get the actual angle
	return acos(quat.w * quat.w - quat.x * quat.x - quat.y * quat.y + quat.z * quat.z);
}