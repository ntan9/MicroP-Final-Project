// STM32F401RE_TIM.h
// Header for TIM functions

#ifndef STM32F4_TIM_H
#define STM32F4_TIM_H

#include <stdint.h> // Include stdint header
#include "STM32F401RE_GPIO.h"
#include "stm32f4xx.h"

///////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
#define GAME_OVER 12 
#define SUCCESS 4

///////////////////////////////////////////////////////////////////////////////
// Function prototypes
///////////////////////////////////////////////////////////////////////////////

void initTIM(TIM_TypeDef * TIMx);
void configMusicTIM(TIM_TypeDef * TIMx);
void setFreq(TIM_TypeDef * TIMx, int freq);
void delay_millis(TIM_TypeDef * TIMx, uint32_t ms);
void delay_micros(TIM_TypeDef * TIMx, uint32_t us);
void playMusic(int mode);

#endif