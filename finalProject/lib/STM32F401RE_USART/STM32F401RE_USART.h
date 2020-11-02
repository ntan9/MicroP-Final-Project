// STM32F401RE_USART.h
// Header for USART functions

#ifndef STM32F4_USART_H
#define STM32F4_USART_H

#include <stdint.h>
#include "stm32f4xx.h"

///////////////////////////////////////////////////////////////////////////////
// Definitions
///////////////////////////////////////////////////////////////////////////////

// Defines for USART case statements
#define USART1_ID   1
#define USART2_ID   2

///////////////////////////////////////////////////////////////////////////////
// Function prototypes
///////////////////////////////////////////////////////////////////////////////

USART_TypeDef * initUSART(uint8_t USART_ID, uint32_t baud_rate);
void sendChar(USART_TypeDef * USART, uint8_t data);
void sendString(USART_TypeDef * USART, uint8_t * charArray);
void receiveString(USART_TypeDef * USART, uint8_t * charArray);

#endif