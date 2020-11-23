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
#define PUSH_IT 1
#define SHOUT_IT 2
#define WIRE_IT 3
#define HEAT_IT 4
#define SHAKE_IT 5
#define GAME_OVER 6 
#define SUCCESS 7
#define START 8

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