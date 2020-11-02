// STM32F401RE_RCC.h
// Header for RCC functions

#ifndef STM32F4_RCC_H
#define STM32F4_RCC_H

#include <stdint.h>
#include "stm32f4xx.h"

///////////////////////////////////////////////////////////////////////////////
// Definitions
///////////////////////////////////////////////////////////////////////////////

// Global defines related to clock
uint32_t SystemCoreClock;    // Updated by configureClock()
#define HSE_VALUE 8000000    // Value of external input to OSC from ST-LINK

// PLL
#define PLLSRC_HSI 0
#define PLLSRC_HSE 1

// Clock configuration
#define SW_HSI  0
#define SW_HSE  1
#define SW_PLL  2

///////////////////////////////////////////////////////////////////////////////
// Function prototypes
///////////////////////////////////////////////////////////////////////////////

void configurePLL();
void configureClock();

#endif