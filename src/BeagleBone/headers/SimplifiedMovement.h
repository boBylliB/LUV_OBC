#ifndef SIMPLIFIEDMOVEMENT_H
#define SIMPLIFIEDMOVEMENT_H

/* Simplified SBUS Movement
*
* Author: Levi Rushing (3/18/2024)
* Purpose: to convert SBUS control data directly into
*          motor control, for minimum viable movement code
*
* Edited: Sam Sandelin (3/19/2024)
*****/

float normalizeChannel(float channel);
void SBUS2Move(uint16_t* channels, float* motorControl);
void SBUS2Tank(uint16_t* channels, float* motorControl);

#endif
