// STM32F401RE_TIM.c
// TIM functions

#include "STM32F401RE_TIM.h"
// #include "STM32F401RE_RCC.h"

const int success[][2] = {
  {2349, 125}, {2489, 125}, {3135, 125}, {0, 10}
};

const int gameOver[][2] = {
  {155, 375}, {0, 125}, {146, 125}, {0, 125},
  {138, 125}, {0, 125}, {155, 125}, {0, 125},
  {110, 125}, {0, 125}, {98, 500}, {0, 10}
};

const int start[][2] = {
  {262, 250}, {0, 125}, {330, 250}, {349, 125},
  {0, 125}, {330, 125}, {277, 250}, {0, 125},
  {330, 125}, {0, 125}, {392, 125}, {0, 125},
  {523, 250}, {0, 10}
};

const int heat[][2] = {
  {740, 125}, {0, 125}, {740, 125}, {0, 125},
  {740, 125}, {0, 125}, {740, 125}, {0, 125},
  {698, 500}, {784, 500}, {0, 10}
};

const int push[][2] = {
  {123, 125}, {0, 125}, {117, 125}, {131, 125},
  {0, 125}, {110, 250}, {0, 10}
};

const int shout[][2] = {
  {587, 500}, {1480, 125}, {1397, 125}, {1480, 125},
  {1397, 125}, {1480, 125}, {1397, 375}, {0, 10}
};

const int shake[][2] = {
  {392, 50}, {0, 100}, {392, 50}, {0, 100},
  {392, 50}, {0, 100}, {392, 50}, {0, 100},
  {392, 50}, {0, 100}, {392, 50}, {0, 100},
  {392, 50}, {0, 100}, {392, 50}, {0, 100},
};

const int wire[][2] = {
  {831, 125}, {0, 125}, {831, 125}, {0, 125},
  {466, 125}, {0, 125}, {523, 125}, {0, 375},
  {349, 125}, {0, 125}, {1047, 125}, {0, 125},
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
  // Enable TIM2 counter and select CLK_INT as timer clock
    TIMx->CR1 |= 1;

    // Initialize TIM2 attributes
    TIMx->CCR1 = 5;
    TIMx->ARR = 4;
    TIMx->PSC = 2;

    // Config CH1
    TIMx->CCMR1 |= (0b111 << 4);       // Set PWM mode 2 -> (ARR >= CCR1) then PWM is on  
    TIMx->CCER |= 1;                   // Enable CH1

    TIMx->CR1 |= (1 << 7);             // Allow ARR reg to be buffered
    TIMx->EGR |= 1;               // Initialize preloaded registers   
}

void setFreq(TIM_TypeDef * TIMx, int freq) {
  // If freq is 0, set CCR1 to be greater than ARR
    if (freq == 0) {
        TIMx->CCR1 = TIMx->ARR + 1; 
    } else {
        // Adjust ARR and CCR1 to create the intended freq
        int clk = 28000000;
        int divider = clk / freq;
        TIMx->ARR = divider;
        TIMx->CCR1 = divider / 2;
    }
}

// Play music through the TIM5 timer based on which mode
void playMusic(int mode) {
  switch (mode) {
  case GAME_OVER:
    for (int i = 0; i < 12; ++i) {
      setFreq(TIM5, gameOver[i][0]);
      delay_millis(TIM2, gameOver[i][1]);
    }
    break;
  case SUCCESS:
    for (int i = 0; i < 4; ++i) {
      setFreq(TIM5, success[i][0]);
      delay_millis(TIM2, success[i][1]);
    }
    break;
  case START:
    for (int i = 0; i < 14; ++i) {
      setFreq(TIM5, start[i][0]);
      delay_millis(TIM2, start[i][1]);
    }
    break;
  case HEAT_IT:
    for (int i = 0; i < 11; ++i) {
      setFreq(TIM5, heat[i][0]);
      delay_millis(TIM2, heat[i][1]);
    }
    break;
  case PUSH_IT:
    for (int i = 0; i < 7; ++i) {
      setFreq(TIM5, push[i][0]);
      delay_millis(TIM2, push[i][1]);
    }
    break;
  case SHOUT_IT:
    for (int i = 0; i < 8; ++i) {
      setFreq(TIM5, shout[i][0]);
      delay_millis(TIM2, shout[i][1]);
    }
    break;
  case SHAKE_IT:
    for (int i = 0; i < 16; ++i) {
      setFreq(TIM5, shake[i][0]);
      delay_millis(TIM2, shake[i][1]);
    }
    break;
  case WIRE_IT:
    for (int i = 0; i < 12; ++i) {
      setFreq(TIM5, wire[i][0]);
      delay_millis(TIM2, wire[i][1]);
    }
    break;
  default:
    break;
  }
}