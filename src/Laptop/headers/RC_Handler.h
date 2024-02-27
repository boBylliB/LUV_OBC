/*RC Command Handler
*
* Author: April Paltrineri
* Purpose: to intake an x axis input and a y axis input
*          and convert them to turn rate and drive speed
*****/

// Function to convert x-axis inputs to turn rate
float convertToTurnRate(float xInput);

// Function to convert y-axis inputs to drive speed
float convertToDriveSpeed(float yInput);

// Function to process remote control signals
// NOTE: Will need to be updated in the future to match whatever input data is coming in from the decoded SBUS signal
void processRemoteControlSignals(int signal, float* axis);

void RC_Handler(int remote_control_signal, float* turnDrive);