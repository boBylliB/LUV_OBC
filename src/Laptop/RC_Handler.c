//RC Command Handler

/* Author: April Paltrineri 
* Purpose: to intake an x axis input and a y axis input 
*          and convert them to turn rate and drive speed
*****/ 

#include <stdio.h>

// Function to convert x-axis inputs to turn rate
float convertToTurnRate(float* xInput) {
    // Conversion logic for turn rate calculation
    float turnRate = xInput * 2.0; // Example conversion formula
    return turnRate;
}

// Function to convert y-axis inputs to drive speed
float convertToDriveSpeed(float* yInput) {
    // Conversion logic for drive speed calculation
    float driveSpeed = yInput * 1.5; // Example conversion formula
    return driveSpeed;
}

// Function to process remote control signals
float* processRemoteControlSignals(int signal) {
    int y_axis_input = signal / 10;  // Extracting y-axis input
    int x_axis_input = signal % 10;  // Extracting x-axis input
    float* axis[2];
    axis[0] = &y_axis_input;
    axis[1] = &x_axis_input;
    
    printf("Y-Axis Input: %d\n", y_axis_input);
    printf("X-Axis Input: %d\n", x_axis_input);
    
    return axis;
}

// This is just so that we can see it technically runs //
// insert remote control signal into the following function//
int  remote_control_signal = 400;

float* RC_Handler(int remote_control_signal) {
    // turn signal into axis inputs
    float* axis[2];
    axis = processRemoteControlSignals(remote_control_signal);
    
    // Convert x-axis input to turn rate
    float turnRate = convertToTurnRate(axis[1]);
    printf("Turn Rate: %.2f\n", turnRate);

    // Convert y-axis input to drive speed
    float driveSpeed = convertToDriveSpeed(axis[0]);
    printf("Drive Speed: %.2f\n", driveSpeed);

    float* turnDrive[2];
    turnDrive[0] = &turnRate;
    turnDrive[1] = &driveSpeed;
    
    return turnDrive;
}
