// STM32F401RE_SPI.h
// Header for SPI functions

#ifndef STM32F4_SPI_H
#define STM32F4_SPI_H

#include <stdint.h> // Include stdint header
#include "stm32f4xx.h"

///////////////////////////////////////////////////////////////////////////////
// Function prototypes
///////////////////////////////////////////////////////////////////////////////

/* Enables the SPI peripheral and intializes its clock speed (baud rate), polarity, and phase.
 *    -- clkdivide: (0x01 to 0xFF). The SPI clk will be the master clock / clkdivide.
 *    -- cpol: clock polarity (0: inactive state is logical 0, 1: inactive state is logical 1).
 *    -- cpha: clock phase (1: data changed on leading edge of clk and captured on next edge, 
 *          0: data captured on leading edge of clk and changed on next edge)
 * Note: the SPI mode register is set with the following unadjustable settings:
 *    -- Master mode
 *    -- Fixed peripheral select
 *    -- Chip select lines directly connected to peripheral device
 *    -- Mode fault detection enabled
 *    -- WDRBT disabled
 *    -- LLB disabled
 *    -- PCS = 0000 (Peripheral 0 selected), means NPCS[3:0] = 1110
 * Refer to the datasheet for more low-level details. */ 
void spiInit(uint32_t clkdivide, uint32_t cpol, uint32_t ncpha);

/* Transmits a character (1 byte) over SPI and returns the received character.
 *    -- send: the character to send over SPI
 *    -- return: the character received over SPI */
uint8_t spiSendReceive(uint8_t send);

/* Transmits a short (2 bytes) over SPI and returns the received short.
 *    -- send: the short to send over SPI
 *    -- return: the short received over SPI */
uint16_t spiSendReceive16(uint16_t send);

#endif