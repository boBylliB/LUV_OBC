//Turn SBUS data into Movement.
//I have no idea, =)

#define InputRange = 32768 //defines the input range we could get from channels, came up enough to warrent a define
#include "settings.h"
#include <stdio.h>

float function SBus2Move (uint16_t) {

int n = length(unit16_t), Channel[n],i; //This and the for loop below just to turn unit16_t into an array
for (i = 0; i < n; i++) { 
     Channel[i] = unit16_t[i]; //The channel I want is 0 and 1. To change which one we use change
}
float LeftXChannel = NormChannel(Channel[0]); //The number here for the turnrate
float LeftyChannel = NormChannel(Channel[1]); //And the number here for Drivespeed
float LeftTurnRate = convertToTurnRate(XChannel); %Produces turn rate
float LeftDriveSpeed = convertToDriveSpeed(YChannel);

float RightXChannel = NormChannel(Channel[2]); //The number here for the turnrate
float RightyChannel = NormChannel(Channel[3]); //And the number here for Drivespeed
float RightTurnRate = convertToTurnRate(XChannel); %Produces turn rate
float RightDriveSpeed = convertToDriveSpeed(YChannel);
return float* SignalArray[LeftTurnRate,LeftDriveSpeed,RightTurnRate,RightDriveSpeed];
} //End of function

//Biginnig of subfunctions
float NormChannel (float Channel) {
	float SignCheck = Channel - InputRange;
	float FinalChannel = SifnCheck/InputRange;
return FinalChannel;
}

// Function to convert x-axis inputs to turn rate
float convertToTurnRate(float xInput) {
    // Conversion logic for turn rate calculation
    float turnRate = xInput * (float)(XINPUTTOTURNRATE); // Example conversion formula
    return turnRate;
}

// Function to convert y-axis inputs to drive speed
float convertToDriveSpeed(float yInput) {
    // Conversion logic for drive speed calculation
    float driveSpeed = yInput * (float)(YINPUTTOSPEED); // Example conversion formula
    return driveSpeed;
}

void translateWheelSpeeds(float* wheelSpeeds, float* turnRate, float* linearSpeed) {
	// Average the left and right sides
	float leftSpeed = (wheelSpeeds[0] + wheelSpeeds[1]) / 2;
	float rightSpeed = (wheelSpeeds[2] + wheelSpeeds[3]) / 2;

	// Get the turn rate as the differential speed between the sides
	// In this case, counterclockwise (left) is positive
	*turnRate = rightSpeed - leftSpeed;

	// Get the linear speed as the average between the sides
	*linearSpeed = (leftSpeed + rightSpeed) / 2;
}
