// Standard library includes
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

// Vendor-provided device header file
#include "stm32f4xx.h"
#include "STM32F401RE.h"

int main() {
    configureFlash();
    configureClock();

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;   
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;


    GPIOB->AFR[0] |= (4 << GPIO_AFRL_AFSEL6_Pos | 4 << GPIO_AFRL_AFSEL7_Pos);
    pinMode(GPIOB, 6, 2);
    pinMode(GPIOB, 7, 2);
    GPIOB->OTYPER |= (1 << GPIO_OTYPER_OT6_Pos | 1 << GPIO_OTYPER_OT7_Pos);

    initI2C(I2C1);
    setUpAccelerometer(I2C1);

    while(1) {
        volatile uint8_t result = detectMotion(I2C1);
    }
}