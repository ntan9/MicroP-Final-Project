// Function definitions for the ADC

#include "STM32F401RE_ADC.h"

void initADC(ADC_TypeDef *A2D, GPIO_TypeDef *GPIO, uint8_t pin, uint8_t sample) {
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; // Enable ADC

    A2D->CR2 |= ADC_CR2_ADON;   // Power on ADC
    A2D->CR2 |= ADC_CR2_EOCS;   // Set software conversion management

    A2D->SQR1 &= (0b0000 << ADC_SQR1_L_Pos); // Only do conversion on one channel
    A2D->CR2 |= ADC_CR2_CONT;   // Continuous conversion mode

    // Enable watchdog
    A2D->CR1 |= ADC_CR1_AWDEN;  // Enable watchdog on regular channels
    // A2D->CR1 |= ADC_CR1_JAWDEN;  // Enable watchdog on injected channels
    // A2D->CR1 |= ADC_CR1_AWDSGL;  // Enable watchdog on a single channel in scan mode
    // // Enable watchdog interrupt enable
    // A2D->CR1 |= ADC_CR1_AWDIE;
    // Set watchdog on input channel 0
    A2D->CR1 &= ~(ADC_CR1_AWDCH_Msk);

    // Calculate ADC IN based on GPIO and pin
    if(GPIO == GPIOB) pin += 8;              
    else if(GPIO == GPIOC) pin += 10;
    A2D->SQR3 |= (pin << ADC_SQR3_SQ1_Pos);
    A2D->SMPR2 |= (sample << ADC_SMPR2_SMP0);
}

void calibrate_ADC(ADC_TypeDef *A2D) {
    uint16_t baseline = read_ADC(A2D);
    A2D->HTR &= ~(ADC_HTR_HT_Msk);
    A2D->HTR |= ((baseline + ADC_THRESHOLD_CHANGE) & ADC_HTR_HT_Msk);
    A2D->LTR &= ~(ADC_LTR_LT_Msk);
    A2D->LTR |= ((baseline - ADC_THRESHOLD_CHANGE) & ADC_LTR_LT_Msk);
    
    // Enable watchdog interrupt enable
    A2D->CR1 |= ADC_CR1_AWDIE;
    A2D->SR &= ~(ADC_SR_AWD_Msk);
}

void begin_ADC_conversion(ADC_TypeDef *A2D) {
    A2D->CR2 |= ADC_CR2_CONT;       // enable continuous conversion
    A2D->CR2 |= ADC_CR2_SWSTART;    // begin conversion
}

void stop_ADC_conversion(ADC_TypeDef *A2D) {
    A2D->CR2 &= ~(ADC_CR2_CONT_Msk);
}

uint16_t read_ADC(ADC_TypeDef *A2D) {
    A2D->CR2 |= ADC_CR2_SWSTART;        // Begin conversion of ADC
    while(!(A2D->SR & ADC_SR_EOC));     // Wait until conversion is complete
    return (A2D->DR & ADC_DR_DATA);
}