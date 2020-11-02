// STM32F401RE_SPI.c
// SPI function declarations

#include "STM32F401RE_SPI.h"
#include "STM32F401RE_RCC.h"
#include "STM32F401RE_GPIO.h"

/* Enables the SPI peripheral and intializes its clock speed (baud rate), polarity, and phase.
 *    -- br: (0b000 - 0b111). The SPI clk will be the master clock / 2^(BR+1).
 *    -- cpol: clock polarity (0: inactive state is logical 0, 1: inactive state is logical 1).
 *    -- ncpha: clock phase (0: data changed on leading edge of clk and captured on next edge, 
 *          1: data captured on leading edge of clk and changed on next edge)
 * Note: the SPI mode register is set with the following unadjustable settings:
 *    -- Master mode
 *    -- Fixed peripheral select
 *    -- Chip select lines directly connected to peripheral device
 *    -- Mode fault detection enabled
 *    -- WDRBT disabled
 *    -- LLB disabled
 *    -- PCS = 0000 (Peripheral 0 selected), means NPCS[3:0] = 1110
 * Refer to the datasheet for more low-level details. */ 
void spiInit(uint32_t br, uint32_t cpol, uint32_t cpha) {
    // Turn on GPIOA and GPIOB clock domains (GPIOAEN and GPIOBEN bits in AHB1ENR)
    RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN);
    
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; // Turn on SPI1 clock domain (SPI1EN bit in APB2ENR)
    // Initially assigning SPI pins
    pinMode(GPIOA, 5, GPIO_ALT); // PA5, Arduino D13, SPI1_SCK
    pinMode(GPIOA, 7, GPIO_ALT); // PA7, Arduino D11, SPI1_MOSI
    pinMode(GPIOA, 4, GPIO_ALT); // PA4, Arduino A2, SPI1_NSS
    pinMode(GPIOB, 6, GPIO_OUTPUT); // PB6, Arduino D10, Manual CS

    // Set to AF05 for SPI alternate functions
    GPIOA->AFR[0] |= (5 << GPIO_AFRL_AFSEL5_Pos | 5 << GPIO_AFRL_AFSEL7_Pos | 5 << GPIO_AFRL_AFSEL4_Pos);

    // Set all relevant bits to 0
    SPI1->CR1 &= ~(SPI_CR1_BR_Msk | SPI_CR1_CPOL_Msk | SPI_CR1_CPHA_Msk | SPI_CR1_LSBFIRST_Msk |
                SPI_CR1_DFF_Msk | SPI_CR1_SSM_Msk | SPI_CR1_MSTR_Msk | SPI_CR1_SPE_Msk);
    
    // Set clock divisor, polarity, phase, 16 bit format, put SPI in master mode
    SPI1->CR1 |= (br << SPI_CR1_BR_Pos | cpol << SPI_CR1_CPOL_Msk | cpha << SPI_CR1_CPHA_Pos |
                1 << SPI_CR1_DFF_Pos | 1 << SPI_CR1_MSTR_Pos);
    // Set NSS pin to output mode
    SPI1->CR2 |= SPI_CR2_SSOE;
    // Enable SPI
    SPI1->CR1 |= SPI_CR1_SPE_Pos;
}

// /* Transmits a character (1 byte) over SPI and returns the received character.
//  *    -- send: the character to send over SPI
//  *    -- return: the character received over SPI */
// uint8_t spiSendReceive(uint8_t send) {
//     SPI1->DR.DR = send; // Transmit the character over SPI
//     while (!(SPI->SPI_SR.RDRF)); // Wait until data has been received
//     return (char) (SPI->SPI_RDR.RD); // Return received character

//     SPI1->DR = send;
//     while (!(SPI1->))
// }

/* Transmits a short (2 bytes) over SPI and returns the received short.
 *    -- send: the short to send over SPI
 *    -- return: the short received over SPI */
uint16_t spiSendReceive16(uint16_t send) {
    digitalWrite(GPIOB, 6, 0);
    SPI1->CR1 |= SPI_CR1_SPE;
    SPI1->DR = send;

    while(!(SPI1->SR & SPI_SR_RXNE));
    uint16_t rec = SPI1->DR & SPI_DR_DR_Msk;
    
    SPI1->CR1 &= ~(SPI_CR1_SPE_Msk);
    digitalWrite(GPIOB, 6, 1);

    return rec;
}