// Function definitions for the ADC

#include "STM32F401RE_ADC.h"

void initADC(ADC_TypeDef *A2D, GPIO_TypeDef *GPIO, uint8_t pin) {
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; // Enable ADC

    A2D->CR2 |= ADC_CR2_ADON;   // Power on ADC
    A2D->CR2 |= ADC_CR2_EOCS;   // Set software conversion management

    A2D->SQR1 &= (0b0000 << ADC_SQR1_L_Pos); // Do only one conversion

    // Calculate ADC IN based on GPIO and pin
    if(GPIO == GPIOB) pin += 8;              
    else if(GPIO == GPIOC) pin += 10;
    A2D->SQR3 |= (pin << ADC_SQR3_SQ1_Pos);
}



uint16_t read_ADC(ADC_TypeDef *A2D) {
    A2D->CR2 |= ADC_CR2_SWSTART;        // Begin conversion of ADC
    while(!(A2D->SR & ADC_SR_EOC));   // Wait until conversion is complete
    return (A2D->DR & ADC_DR_DATA);
}