/*RC Command Handler
*
* Author: April Paltrineri (2/23/2024)
* Purpose: to intake an x axis input and a y axis input
*          and convert them to turn rate and drive speed
*
* Edited: Sam Sandelin (2/27/2024)
*****/

#include <stdio.h>

// Function to convert x-axis inputs to turn rate
float convertToTurnRate(float xInput) {
    // Conversion logic for turn rate calculation
    float turnRate = xInput * (float)2.0; // Example conversion formula
    return turnRate;
}

// Function to convert y-axis inputs to drive speed
float convertToDriveSpeed(float yInput) {
    // Conversion logic for drive speed calculation
    float driveSpeed = yInput * (float)1.5; // Example conversion formula
    return driveSpeed;
}

// Function to process remote control signals
// NOTE: Will need to be updated in the future to match whatever input data is coming in from the decoded SBUS signal
void processRemoteControlSignals(int signal, float* axis) {
    int y_axis_input = signal / 10;  // Extracting y-axis input
    int x_axis_input = signal % 10;  // Extracting x-axis input

    axis[0] = (float)y_axis_input;
    axis[1] = (float)x_axis_input;

    printf("Y-Axis Input: %d\n", y_axis_input);
    printf("X-Axis Input: %d\n", x_axis_input);
}

void RC_Handler(int remote_control_signal, float* turnDrive) {
    // turn signal into axis inputs
    float axis[2];
    processRemoteControlSignals(remote_control_signal, axis);

    // Convert x-axis input to turn rate
    float turnRate = convertToTurnRate(axis[1]);
    printf("Turn Rate: %.2f\n", turnRate);

    // Convert y-axis input to drive speed
    float driveSpeed = convertToDriveSpeed(axis[0]);
    printf("Drive Speed: %.2f\n", driveSpeed);

    // Setting output parameters
    turnDrive[0] = turnRate;
    turnDrive[1] = driveSpeed;
}
