// Header file for ADC library

#ifndef STM32F4_I2C_H
#define STM32F4_I2C_H

#include <stdint.h>
#include "stm32f4xx.h"

void initADC(ADC_TypeDef *A2D);
uint16_t read_ADC(ADC_TypeDef *A2D);

#endif