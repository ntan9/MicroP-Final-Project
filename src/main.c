// Standard library includes
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

// Vendor-provided device header file
#include "stm32f4xx.h"
#include "STM32F401RE.h"

#define PUSH_BUTTON 13

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
    initTIM(TIM2);                              // Initialize TIM2
    initADC(ADC1, GPIOA, 0, 6);                 // Initialize ADC to pin A0
    initPushButton();

    pinMode(GPIOA, 0, GPIO_ANALOG);             // PA0 is input for ADC
    pinMode(GPIOC, 13, GPIO_INPUT);             // PC13 is Nucleo Push Button

    // Enable interrupts globally
    __enable_irq();


    while (1) {
        // adc_val = read_ADC(ADC1);
        // sprintf(msg, "ADC Value: %d\n\r", adc_val);
        // sendString(USART2, msg);
        delay_millis(TIM2, 250);
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
        sendString(USART2, "Push it!\n\r");
    }
}