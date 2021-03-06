// main.h
// Standard library includes
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

// Vendor-provided device header file
#include "stm32f4xx.h"
#include "STM32F401RE.h"


////////////////////////////
// Definitions
////////////////////////////

#ifndef max
    #define max(a, b) ((a) > (b) ? (a) : (b))
#endif


#define PUSH_BUTTON 13
#define WIRE_IT_PIN 5
#define DELAY_TIM TIM2
#define SOUND_TIM TIM5

#define PUSH_IT 1
#define SHOUT_IT 2
#define WIRE_IT 3
#define HEAT_IT 4
#define SHAKE_IT 5

#define NUM_COMMANDS 5

#define MESSAGE_DELAY 1500
#define GAME_DELAY 5000
#define GAME_DELAY_CHANGE -15
#define MIN_GAME_DELAY 500


/////////////////////////////
// Function Prototypes
////////////////////////////

void initPushButton();
void waitForInput(uint32_t gameDelay);
void displayInit(void);
void EXTI15_10_IRQHandler(void);
void ADC_IRQHandler(void);