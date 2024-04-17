#ifndef BNO08X_H
#define BNO08X_H

/* BNO08X
*
* Author: Sam Sandelin (4/8/2024)
* Purpose: Communication handler for connected BNO08X IMUs
*
* Pulled and modified from the Adafruit BNO08x library for Arduino (https://github.com/adafruit/Adafruit_BNO08x),
* just converted to only use SPI and run in pure C
*/

#include "sh2.h"
#include "sh2_SensorValue.h"
#include "sh2_err.h"
#include "Quaternion.h"

/* Additional Activities not listed in SH-2 lib */
#define PAC_ON_STAIRS 8 ///< Activity code for being on stairs
#define PAC_OPTION_COUNT 9 ///< The number of current options for the activity classifier

typedef struct {
    sh2_ProductIds_t prodIds;
    sh2_Hal_t _HAL;
} BNO08x;

uint8_t GetOrientation(BNO08x* imuControl, sh2_SensorValue_t* sensorReport, Quaternion_t* quat);
uint8_t begin_SPI(BNO08x* bno08x, uint8_t cs_pin, uint8_t int_pin, uint8_t rst_pin, int32_t sensor_id);
void hardwareReset();
uint8_t wasReset(BNO08x* bno08x);
uint8_t enableReport(BNO08x* bno08x, sh2_SensorId_t sensor, uint32_t interval_us);
uint8_t getSensorEvent(BNO08x* bno08x, sh2_SensorValue_t* value);

uint8_t __init(BNO08x* bno08x, int32_t sensor_id);

#endif
