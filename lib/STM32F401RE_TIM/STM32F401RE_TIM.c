// STM32F401RE_TIM.c
// TIM functions

#include "STM32F401RE_TIM.h"
#include "STM32F401RE_RCC.h"

const int success[][2] = {
  {2349, 125}, {2489, 125}, {3135, 125}, {0, 10}
};

const int gameOver[][2] = {
  {155, 375}, {0, 125}, {146, 125}, {0, 125},
  {138, 125}, {0, 125}, {155, 125}, {0, 125},
  {110, 125}, {0, 125}, {98, 500}, {0, 10}
};


void initTIM(TIM_TypeDef * TIMx){
  // Set prescaler to give 1 µs time base
  uint32_t psc_div = (uint32_t) ((SystemCoreClock/1e6)-1);

  // Set prescaler division factor
  TIMx->PSC = (psc_div - 1);
  // Generate an update event to update prescaler value
  TIMx->EGR |= TIM_EGR_UG;
  // Enable counter
  TIMx->CR1 |= TIM_CR1_CEN; // Set CEN = 1
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
  // Enable TIM counter and select CLK_INT as timer clock
    TIMx->CR1 |= TIM_CR1_CEN;

    // Initialize TIM2 attributes
    TIMx->CCR4 = 5;
    TIMx->ARR = 4;
    TIMx->PSC = 2;

    // Config CH4
    TIMx->CCMR2 |= (0b111 << TIM_CCMR2_OC4M_Pos);   // Set PWM mode 2 -> (ARR >= CCR4) then PWM is on
    TIMx->CCMR2 |= TIM_CCMR2_OC4PE;                 // Preload register on CCR4
    TIMx->CCER |= TIM_CCER_CC4E;                    // Enable CH4

    TIMx->CR1 |= TIM_CR1_ARPE;           // Allow ARR reg to be buffered 

    TIMx->EGR |= TIM_EGR_UG;             // Initialize preloaded registers   
}

void setFreq(TIM_TypeDef * TIMx, int freq) {
  // If freq is 0, set CCR1 to be greater than ARR
    if (freq == 0) {
        TIMx->CCR4 = TIMx->ARR + 1; 
    } else {
        // Adjust ARR and CCR1 to create the intended freq
        int clk = SystemCoreClock/3;
        int divider = clk / freq;
        TIMx->ARR = divider;
        TIMx->CCR4 = divider / 2;
    }
    TIMx->EGR |= TIM_EGR_UG;                          // Initialize preloaded registers
}

// Play music through the TIM5 timer based on which mode
void playMusic(int mode) {
  switch (mode) {
  case GAME_OVER:
    for (int i = 0; i < GAME_OVER; ++i) {
      setFreq(TIM5, gameOver[i][0]);
      delay_millis(TIM2, gameOver[i][1]);
    }
    break;
  case SUCCESS:
    for (int i = 0; i < SUCCESS; ++i) {
      setFreq(TIM5, success[i][0]);
      delay_millis(TIM2, success[i][1]);
    }
    break;
  default:
    break;
  }
}