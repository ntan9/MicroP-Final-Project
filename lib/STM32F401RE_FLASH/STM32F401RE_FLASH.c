// STM32F401RE_FLASH.c
// Source code for FLASH functions

#include "STM32F401RE_FLASH.h"

void configureFlash() {
    // Set waitstates and turn on the ART
    FLASH->ACR &= ~(FLASH_ACR_LATENCY_Msk | FLASH_ACR_PRFTEN_Msk);
    FLASH->ACR |= (2 << FLASH_ACR_LATENCY_Pos | 1 << FLASH_ACR_PRFTEN_Pos);
}