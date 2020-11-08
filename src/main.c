// Standard library includes
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

// Vendor-provided device header file
#include "stm32f4xx.h"
#include "STM32F401RE.h"

#define PUSH_BUTTON 13
#define DELAY_TIM TIM2

#define PUSH_IT 1
#define SHOUT_IT 2
#define SHAKE_IT 3
#define HEAT_IT 4

#define MESSAGE_DELAY 250
#define GAME_DELAY 2000
#define GAME_DELAY_CHANGE -5

uint8_t gameStarted;
uint8_t gameOver;
uint32_t gameDelay;
uint32_t score;
uint8_t task;
uint8_t input;
/**
 * Main program.
 */
int main(void) {
    uint8_t msg[64];
    uint16_t adc_val;
    configureFlash();
    configureClock();

    RCC->AHB1ENR  |= (RCC_AHB1ENR_GPIOAEN);     // Enable GPIOA
    RCC->AHB1ENR  |= (RCC_AHB1ENR_GPIOCEN);     // Enable GPIOC
    RCC->APB1ENR  |= (RCC_APB1ENR_TIM2EN);      // Enable TIM2
    RCC->APB2ENR  |= (RCC_APB2ENR_SYSCFGEN);    // Enable SYSCFG clock domain

    initUSART(USART2_ID, 115200);               // Initialize USART2 to print to terminal
    initTIM(DELAY_TIM);                              // Initialize TIM2
    initADC(ADC1, GPIOA, 0, 6);                 // Initialize ADC to pin A0
    initPushButton();

    pinMode(GPIOA, 0, GPIO_ANALOG);             // PA0 is input for ADC
    pinMode(GPIOC, 13, GPIO_INPUT);             // PC13 is Nucleo Push Button

    // Enable interrupts globally
    __enable_irq();

    gameStarted = 0;
    input = 0;

    while (1) {
        delay_millis(DELAY_TIM, MESSAGE_DELAY);
        sendString(USART2, "Push button to begin!\n\r");
        // adc_val = read_ADC(ADC1);
        // sprintf(msg, "ADC Value: %d\n\r", adc_val);
        // sendString(USART2, msg);



        // Wait for user to start the game
        while(!gameStarted){
            /* 
             * For some reason, without the delay line the interrupt doesn't trigger
             * I think this has to do with the compiler optimizing parts out
             */
            delay_millis(DELAY_TIM, MESSAGE_DELAY);
        }
        score = 0;
        gameOver = 0;
        gameDelay = GAME_DELAY;
        sendString(USART2, "Ready?\n\r");
        delay_millis(DELAY_TIM, MESSAGE_DELAY);
        sendString(USART2, "Begin!\n\r");

        // Main game functionality
        while(1) {
            task = PUSH_IT;
            sendString(USART2, "Push It!\n\r");
            delay_millis(DELAY_TIM, gameDelay);
            if(task != input) goto game_over;
            ++score;
            input = 0;
            gameDelay += GAME_DELAY_CHANGE;
        }
        
game_over:
        // Game Over Handler
        gameStarted = 0;
        sendString(USART2, "You Failed!\n\r");
        sprintf(msg, "Your score was %d!\n\r", score);
        sendString(USART2, msg);
        delay_millis(DELAY_TIM, MESSAGE_DELAY);
    }
}


void initPushButton() {
    // Set EXTICR4 for PC13
    SYSCFG->EXTICR[3] &= ~(SYSCFG_EXTICR4_EXTI13_Msk);
    SYSCFG->EXTICR[3] |= (0b0010 << SYSCFG_EXTICR4_EXTI13_Pos);

    EXTI->IMR |= EXTI_IMR_IM13;             // Configure IM13 mask bit
    EXTI->RTSR &= ~(EXTI_RTSR_TR13_Msk);    // Disable rising edge trigger
    EXTI->FTSR |= EXTI_FTSR_TR13;           // Enable falling edge trigger
    __NVIC_EnableIRQ(40);                   // Enable External Line[15:10] Interrupts
}



/*
 * Interrupts
 */

// Push button interrupt handler
void EXTI15_10_IRQHandler(void){
    // Check that the button EXTI_13 was what triggered our interrupt
    if (EXTI->PR & (1 << PUSH_BUTTON)){
        // Clear Interrupt
        EXTI->PR |= (1 << PUSH_BUTTON);
        if(!gameStarted) {
            gameStarted = 1;
        }
        else {
            input = PUSH_IT;
            DELAY_TIM->EGR |= TIM_EGR_UG;    // Write bit to clear the delay timer
        }
        // sendString(USART2, "Push it!\n\r");
    }
}

