// Standard library includes
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

// Vendor-provided device header file
#include "stm32f4xx.h"
#include "STM32F401RE.h"

#define DMA_STREAM DMA1_Stream6

/**
 * Main program.
 */
int main(void) {
    uint8_t msg[64];
    uint16_t adc_val;
    configureFlash();
    configureClock();

    RCC->AHB1ENR  |= (RCC_AHB1ENR_GPIOAEN);
    RCC->APB1ENR  |= (RCC_APB1ENR_TIM2EN);

    initUSART(USART2_ID, 115200);
    initTIM(TIM2);
    initADC(ADC1, GPIOA, 0);

    while (1) {
        adc_val = read_ADC(ADC1);
        sprintf(msg, "ADC Value: %d\n\r", adc_val);
        sendString(USART2, msg);
        delay_millis(TIM2, 250);
    }
}