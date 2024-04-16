#include "BNO08X.h"
#include "SPI.h"
#include "sh2.h"
#include "sh2_SensorValue.h"
#include "sh2_err.h"
#include "settings.h"
#include "GPIOLookup.h"
#include "GPIOControl.h"
#include "Quaternion.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

uint8_t GetOrientation(BNO08x* imuControl, sh2_SensorValue_t* sensorReport, Quaternion_t* quat) {
	uint8_t status = 0;
	if (getSensorEvent(imuControl, sensorReport) && sensorReport->sensorId == SH2_GAME_ROTATION_VECTOR) {
		sh2_RotationVector_t value = (sh2_RotationVector_t)sensorReport->un;
		quat->w = value.real;
		quat->x = value.i;
		quat->y = value.j;
		quat->z = value.k;
		status = 1;
	}
	return status;
}

// NOTE: Leaving in the following file block as the majority of this file was translated from the Adafruit C++ implementation
/*!
 *  @file Adafruit_BNO08x.cpp
 *
 *  @mainpage Adafruit BNO08x 9-DOF Orientation IMU Fusion Breakout
 *
 *  @section intro_sec Introduction
 *
 * 	I2C Driver for the Library for the BNO08x 9-DOF Orientation IMU Fusion
 * Breakout
 *
 * 	This is a library for the Adafruit BNO08x breakout:
 * 	https://www.adafruit.com/product/4754
 *
 * 	Adafruit invests time and resources providing this open source code,
 *  please support Adafruit and open-source hardware by purchasing products from
 * 	Adafruit!
 *
 *  @section dependencies Dependencies
 *  This library depends on the Adafruit BusIO library
 *
 *  This library depends on the Adafruit Unified Sensor library
 *
 *  @section author Author
 *
 *  Bryan Siepert for Adafruit Industries
 *
 * 	@section license License
 *
 * 	BSD (see license.txt)
 *
 * 	@section  HISTORY
 *
 *     v1.0 - First release
 */

static SPI_t _SPIControl;
static int8_t _int_pin, _reset_pin, _cs_pin;

static sh2_SensorValue_t* _sensor_value = NULL;
static uint8_t _reset_occurred = 0;

static uint8_t spihal_wait_for_int(void);
static int spihal_write(sh2_Hal_t* self, uint8_t* pBuffer, unsigned len);
static int spihal_read(sh2_Hal_t* self, uint8_t* pBuffer, unsigned len,
	uint32_t* t_us);
static void spihal_close(sh2_Hal_t* self);
static int spihal_open(sh2_Hal_t* self);

static uint32_t hal_getTimeUs(sh2_Hal_t* self);
static void hal_callback(void* cookie, sh2_AsyncEvent_t* pEvent);
static void sensorHandler(void* cookie, sh2_SensorEvent_t* pEvent);
static void hal_hardwareReset(void);

/*!
 *    @brief  Sets up the hardware and initializes hardware SPI
 *    @param  cs_pin The arduino pin # connected to chip select
 *    @param  int_pin The arduino pin # connected to BNO08x INT
 *    @param  theSPI The SPI object to be used for SPI connections.
 *    @param  sensor_id
 *            The user-defined ID to differentiate different sensors
 *    @return 1 if initialization was successful, otherwise 0.
 */
uint8_t begin_SPI(BNO08x* bno08x, uint8_t cs_pin, uint8_t int_pin, uint8_t rst_pin, int32_t sensor_id) {
	// Set pin modes
	_int_pin = int_pin;
	_cs_pin = cs_pin;
	_reset_pin = rst_pin;
	// NOTE: Not sure how to set the interrupt pin to be an input pullup, may be an issue, but the datasheet just says its active low
	if (!SetGPIO(_int_pin, DIRECTION, "in")) return 0;
	if (!SetGPIO(_cs_pin, DIRECTION, "out")) return 0;
	if (!SetGPIO(_reset_pin, DIRECTION, "out")) return 0;
	// Set function pointers
	bno08x._HAL.open = spihal_open;
	bno08x._HAL.close = spihal_close;
	bno08x._HAL.read = spihal_read;
	bno08x._HAL.write = spihal_write;
	bno08x._HAL.getTimeUs = hal_getTimeUs;

	return _init(sensor_id);
}

/*!  @brief Initializer for post i2c/spi init
 *   @param sensor_id Optional unique ID for the sensor set
 *   @returns 1 if chip identified and initialized
 */
uint8_t _init(BNO08x* bno08x, int32_t sensor_id) {
	int status;

	hardwareReset();

	// Open SH2 interface (also registers non-sensor event handler.)
	status = sh2_open(&bno08x._HAL, hal_callback, NULL);
	if (status != SH2_OK) {
		return 0;
	}

	// Check connection partially by getting the product id's
	memset(&bno08x.prodIds, 0, sizeof(bno08x.prodIds));
	status = sh2_getProdIds(&bno08x.prodIds);
	if (status != SH2_OK) {
		return 0;
	}

	// Register sensor listener
	sh2_setSensorCallback(sensorHandler, NULL);

	return 1;
}

/**
 * @brief Reset the device using the Reset pin
 *
 */
void hardwareReset(BNO08x* bno08x) { hal_hardwareReset(); }

/**
 * @brief Check if a reset has occured
 *
 * @return 1: a reset has occured 0: no reset has occoured
 */
uint8_t wasReset(BNO08x* bno08x) {
	uint8_t x = _reset_occurred;
	_reset_occurred = 0;

	return x;
}

/**
 * @brief Fill the given sensor value object with a new report
 *
 * @param value Pointer to an sh2_SensorValue_t struct to fil
 * @return 1: The report object was filled with a new report
 * @return 0: No new report available to fill
 */
uint8_t getSensorEvent(BNO08x* bno08x, sh2_SensorValue_t* value) {
	_sensor_value = value;

	value->timestamp = 0;

	sh2_service();

	if (value->timestamp == 0 && value->sensorId != SH2_GYRO_INTEGRATED_RV) {
		// no new events
		return 0;
	}

	return 1;
}

/**
 * @brief Enable the given report type
 *
 * @param sensorId The report ID to enable
 * @param interval_us The update interval for reports to be generated, in
 * microseconds
 * @return 1: success 0: failure
 */
uint8_t enableReport(BNO08x* bno08x, sh2_SensorId_t sensorId,
	uint32_t interval_us) {
	static sh2_SensorConfig_t config;

	// These sensor options are disabled or not used in most cases
	config.changeSensitivityEnabled = 0;
	config.wakeupEnabled = 0;
	config.changeSensitivityRelative = 0;
	config.alwaysOnEnabled = 0;
	config.changeSensitivity = 0;
	config.batchInterval_us = 0;
	config.sensorSpecific = 0;

	config.reportInterval_us = interval_us;
	int status = sh2_setSensorConfig(sensorId, &config);

	if (status != SH2_OK) {
		return 0;
	}

	return 1;
}

static int spihal_open(sh2_Hal_t* self) {
	// Wait for the interrupt pin to drop low
	spihal_wait_for_int();
	// Initialize SPI interface
	uint8_t status = StartSPI(&_SPIControl);
	if (!status)
		return 0;

	return 0;
}

static uint8_t spihal_wait_for_int(void) {
	// Get the start time
	struct timespec start;
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC_RAW, &start);
	uint64_t delta_ms;
	uint8_t intVal;
	do {
		if (!GetGPIO(_int_pin, &intVal)) {
			fprintf(stderr, "Failed to get interrupt pin value!");
			return 0;
		}
		if (!intVal)
			return 1;
		clock_gettime(CLOCK_MONOTONIC_RAW, &now);
		delta_ms = (now.tv_sec - start.tv_sec) * 1000 + (now.tv_nsec - start.tv_nsec) / 1000000;
	} while (delta_ms < BNO08XINTTIMEOUTMS);

	// If we get here, we timed out
	fprintf(stderr, "Timed out waiting for BNO08x interrupt pin!\n");
	hal_hardwareReset();

	return 0;
}

static void spihal_close(sh2_Hal_t* self) {
	// Close SPI interface
	StopSPI(&_SPIControl);
}

static int spihal_read(sh2_Hal_t* self, uint8_t* pBuffer, unsigned len,
	uint32_t* t_us) {
	uint16_t packet_size = 0;

	if (!spihal_wait_for_int()) {
		return 0;
	}
	// Set chip select pin
	SetGPIO(_cs_pin, VALUE, "1");

	if (!ReadSPI(&_SPIControl, pBuffer, 4)) {
		return 0;
	}

	// Determine amount to read
	packet_size = (uint16_t)pBuffer[0] | (uint16_t)pBuffer[1] << 8;
	// Unset the "continue" bit
	packet_size &= ~0x8000;

	/*
	Serial.print("Read SHTP header. ");
	Serial.print("Packet size: ");
	Serial.print(packet_size);
	Serial.print(" & buffer size: ");
	Serial.println(len);
	*/

	if (packet_size > len) {
		return 0;
	}

	if (!spihal_wait_for_int()) {
		return 0;
	}

	if (!ReadSPI(&_SPIControl, pBuffer, packet_size)) {
		return 0;
	}
	// Unset chip select pin
	SetGPIO(_cs_pin, VALUE, "0");

	return packet_size;
}

static int spihal_write(sh2_Hal_t* self, uint8_t* pBuffer, unsigned len) {
	if (!spihal_wait_for_int()) {
		return 0;
	}

	// Set chip select pin
	SetGPIO(_cs_pin, VALUE, "1");
	// Write
	WriteSPI(&_SPIControl, pBuffer, len);
	// Unset chip select pin
	SetGPIO(_cs_pin, VALUE, "0");

	return len;
}

/**************************************** HAL interface
 * ***********************************************************/

static void hal_hardwareReset(void) {
	// Get the start time
	struct timespec start;
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC_RAW, &start);
	uint64_t delta_ms;
	if (_reset_pin > 0) {
		SetGPIO(_reset_pin, VALUE, "1");
		do {
			clock_gettime(CLOCK_MONOTONIC_RAW, &now);
			delta_ms = (now.tv_sec - start.tv_sec) * 1000 + (now.tv_nsec - start.tv_nsec) / 1000000;
		} while (delta_ms < 10);
		SetGPIO(_reset_pin, VALUE, "0");
		do {
			clock_gettime(CLOCK_MONOTONIC_RAW, &now);
			delta_ms = (now.tv_sec - start.tv_sec) * 1000 + (now.tv_nsec - start.tv_nsec) / 1000000;
		} while (delta_ms < 10);
		SetGPIO(_reset_pin, VALUE, "1");
		do {
			clock_gettime(CLOCK_MONOTONIC_RAW, &now);
			delta_ms = (now.tv_sec - start.tv_sec) * 1000 + (now.tv_nsec - start.tv_nsec) / 1000000;
		} while (delta_ms < 10);
	}
}

static uint32_t hal_getTimeUs(sh2_Hal_t* self) {
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC_RAW, &now);
	uint32_t now_us = now.tv_sec * 1000000 + now.tv_nsec / 1000;
	return now_us;
}

static void hal_callback(void* cookie, sh2_AsyncEvent_t* pEvent) {
	// If we see a reset, set a flag so that sensors will be reconfigured.
	if (pEvent->eventId == SH2_RESET) {
		// Serial.println("Reset!");
		_reset_occurred = 1;
	}
}

// Handle sensor events.
static void sensorHandler(void* cookie, sh2_SensorEvent_t* event) {
	int rc;

	// Serial.println("Got an event!");

	rc = sh2_decodeSensorEvent(_sensor_value, event);
	if (rc != SH2_OK) {
		fprintf(stderr, "BNO08x - Error decoding sensor event\n");
		_sensor_value->timestamp = 0;
		return;
	}
}