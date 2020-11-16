// Header file for ADC library

#ifndef STM32F4_I2C_H
#define STM32F4_I2C_H

#include <stdint.h>
#include "stm32f4xx.h"

#define ADC_THRESHOLD_CHANGE 500
/* Initialize ADC A2D to take input from a GPIO pin
 * And sample for a set time
 * Values for sample:
 *      0 = 3 cycles
 *      1 = 15 cycles
 *      2 = 28 cycles
 *      3 = 56 cycles
 *      4 = 84 cycles
 *      5 = 112 cycles
 *      6 = 144 cycles
 *      7 = 480 cycles
 */
void initADC(ADC_TypeDef *A2D, GPIO_TypeDef *GPIO, uint8_t pin, uint8_t sample);
void calibrateADC(ADC_TypeDef *A2D);
void begin_ADC_conversion(ADC_TypeDef *A2D);
void stop_ADC_conversion(ADC_TypeDef *A2D);
uint16_t read_ADC(ADC_TypeDef *A2D);

#endif