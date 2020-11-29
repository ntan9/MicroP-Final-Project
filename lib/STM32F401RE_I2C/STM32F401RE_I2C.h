// STM32F401RE_I2C.h
#ifndef STM32F4_I2C_H
#define STM32F4_I2C_H

#include <stdint.h>
#include "stm32f4xx.h"

///////////////////////////////////////////////////////////////////////////////
// Definitions
///////////////////////////////////////////////////////////////////////////////
#define DEVICE 0b0011101        // Address for MMA8452Q

///////////////////////////////////////////////////////////////////////////////
// Function prototypes
///////////////////////////////////////////////////////////////////////////////

// Initializes I2C Peripheral
void initI2C(I2C_TypeDef * I2Cx);

// Writes a byte (data) to the reg address
void writeI2C(I2C_TypeDef * I2Cx, uint8_t reg, uint8_t data);

// Reads and returns a byte from reg address
uint8_t readI2C(I2C_TypeDef * I2Cx, uint8_t reg);

// Configures the acceleromter to 
void setUpAccelerometer(I2C_TypeDef * I2Cx);

// Returns 1 if motion detected on accelerometer; 0 otherwise
uint8_t detectMotion(I2C_TypeDef * I2Cx);

#endif