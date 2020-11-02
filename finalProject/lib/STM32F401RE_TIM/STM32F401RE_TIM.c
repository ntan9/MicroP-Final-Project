// STM32F401RE_TIM.c
// TIM functions

#include "STM32F401RE_TIM.h"
#include "STM32F401RE_RCC.h"

void initTIM(TIM_TypeDef * TIMx){
  // Set prescaler to give 1 µs time base
  uint32_t psc_div = (uint32_t) ((SystemCoreClock/1e6)-1);

  // Set prescaler division factor
  TIMx->PSC = (psc_div - 1);
  // Generate an update event to update prescaler value
  TIMx->EGR |= 1 << TIM_EGR_UG_Pos;
  // Enable counter
  TIMx->CR1 |= 1 << TIM_CR1_CEN_Pos; // Set CEN = 1
}

void delay_millis(TIM_TypeDef * TIMx, uint32_t ms){
  TIMx->ARR = ms*1000;// Set timer max count
  TIMx->EGR |= 1 << TIM_EGR_UG_Pos;     // Force update
  TIMx->SR &= ~(TIM_SR_UIF_Msk); // Clear UIF
  TIMx->CNT = 0;      // Reset count

  while(!(TIMx->SR & 1)); // Wait for UIF to go high
}

void delay_micros(TIM_TypeDef * TIMx, uint32_t us){
  TIMx->ARR = us;// Set timer max count
  TIMx->EGR |= 1 << TIM_EGR_UG_Pos;     // Force update
  TIMx->SR &= ~(TIM_SR_UIF_Msk); // Clear UIF
  TIMx->CNT = 0;      // Reset count

  while(!(TIMx->SR & 1)); // Wait for UIF to go high
}

void configMusicTIM(TIM_TypeDef * TIMx) {
  // Enable TIM2 counter and select CLK_INT as timer clock
    TIMx->CR1 |= 1 << TIM_CR1_CEN_Pos;

    // Initialize TIM2 attributes
    TIMx->CCR1 = 5;
    TIMx->ARR = 4;
    TIMx->PSC = 2;

    // Config CH1
    TIMx->CCMR1 |= (0b111 << TIM_CCMR1_OC1M_Pos);   // Set PWM mode 2 -> (ARR >= CCR1) then PWM is on         
    TIMx->CCER |= (1 << TIM_CCER_CC1E);   // Enable CH1

    TIMx->CR1 |= (1 << TIM_ARR_ARR_Pos);  // Allow ARR reg to be buffered        
    TIMx->EGR |= 1 << TIM_EGR_UG_Pos;             // Initialize preloaded registers   
}

void setFreq(TIM_TypeDef * TIMx, int freq) {
  // If freq is 0, set CCR1 to be greater than ARR
    if (freq == 0) {
        TIMx->CCR1 = TIMx->ARR + 1; 
    } else {
        // Adjust ARR and CCR1 to create the intended freq
        int clk = SystemCoreClock;
        int divider = clk / freq;
        TIMx->ARR = divider;
        TIMx->CCR1 = divider / 2;
    }
}