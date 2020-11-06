// STM32F401RE_RCC.c
// Source code for RCC functions

#include "STM32F401RE_RCC.h"

void configurePLL() {
    // Set clock to 84 MHz
    // Output freq = (src_clk) * (N/M) / P
    // (8 MHz) * (336/8) / 4 = 84 MHz
    // M:8, N:336, P:4, Q:7
    // Use HSE as PLLSRC

    RCC->CR &= ~(RCC_CR_PLLON_Msk);     // Turn off PLL

    while ((RCC->CR & RCC_CR_PLLRDY_Msk) != 0);   // Wait till PLL is unlocked

    // Load configuration
    RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLSRC_Msk |
                    RCC_PLLCFGR_PLLM_Msk | RCC_PLLCFGR_PLLN_Msk |
                    RCC_PLLCFGR_PLLP_Msk | RCC_PLLCFGR_PLLQ_Msk);
    
    RCC->PLLCFGR |= (PLLSRC_HSE << RCC_PLLCFGR_PLLSRC_Pos |
                    8 << RCC_PLLCFGR_PLLM_Pos | 336 << RCC_PLLCFGR_PLLN_Pos |
                    0b01 << RCC_PLLCFGR_PLLP_Pos | 4 << RCC_PLLCFGR_PLLQ_Pos);


    // Enable PLL and wait until it's locked
    RCC->CR |= (1 << RCC_CR_PLLON_Pos);
    while(RCC->CR & RCC_CR_PLLRDY_Msk == 0);
}

void configureClock(){
    /* Configure APB prescalers
    1. Set APB2 (high-speed bus) prescaler to no division
    2. Set APB1 (low-speed bus) to divide by 2.
    */
    
    RCC->CFGR &= ~(RCC_CFGR_PPRE1_Msk | RCC_CFGR_PPRE2_Msk);
    RCC->CFGR |= (0b000 << RCC_CFGR_PPRE1_Pos | 0b100 << RCC_CFGR_PPRE2_Pos);
    
    // Turn on and bypass for HSE from ST-LINK
    RCC->CR |= (1 << RCC_CR_HSEBYP_Pos | 1 << RCC_CR_HSEON_Pos);
    while(!RCC->CR & RCC_CR_HSERDY_Msk);
    
    // Configure and turn on PLL for 84 MHz
    configurePLL();

    // Select PLL as clock source
    RCC->CFGR &= ~(RCC_CFGR_SW_Msk);
    RCC->CFGR |= (SW_PLL << RCC_CFGR_SW_Pos);
    while(RCC->CFGR & RCC_CFGR_SWS_Msk != 0b10 << RCC_CFGR_SWS_Pos);

    SystemCoreClock = 84000000;
}