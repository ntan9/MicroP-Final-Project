// STM32F401RE_SPI.c
// SPI function declarations

#include "STM32F401RE_SPI.h"
#include "STM32F401RE_RCC.h"
#include "STM32F401RE_GPIO.h"
#include "bitmaps.h"
// #include "main.h"


char DISPLAYMEM[DISPLAY_WIDTH * DISPLAY_HEIGHT] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

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
    pinMode(GPIOB, 3, GPIO_ALT); // PB3, Arduino D13, SPI1_SCK
    pinMode(GPIOA, 7, GPIO_ALT); // PA7, Arduino D11, SPI1_MOSI
    pinMode(GPIOA, DISPLAY_CS, GPIO_OUTPUT); // Default: PA4, Arduino A2, Manual CS
    pinMode(GPIOB, DISPLAY_DC, GPIO_OUTPUT); // Default: PB5, Arduino D4, Manual D/C select (DISPLAY)
    pinMode(GPIOB, DISPLAY_RESET, GPIO_OUTPUT); // Default: PB4, Arduino D5, Manual Reset (DISPLAY)
    // pinMode(GPIOA, 0, GPIO_OUTPUT);

    // Set to AF05 for SPI alternate functions
    GPIOA->AFR[0] |= (5 << GPIO_AFRL_AFSEL7_Pos);
    GPIOB->AFR[0] |= (5 << GPIO_AFRL_AFSEL3_Pos);
    // Set all relevant bits to 0
    SPI1->CR1 &= ~(SPI_CR1_BR_Msk | SPI_CR1_CPOL_Msk | SPI_CR1_CPHA_Msk | SPI_CR1_LSBFIRST_Msk |
                SPI_CR1_DFF_Msk | SPI_CR1_SSM_Msk | SPI_CR1_MSTR_Msk | SPI_CR1_SPE_Msk);
    
    // Set clock divisor, polarity, phase, 8 bit format, put SPI in master mode
    SPI1->CR1 |= (br << SPI_CR1_BR_Pos | cpol << SPI_CR1_CPOL_Pos | cpha << SPI_CR1_CPHA_Pos |
                 0 << SPI_CR1_DFF_Pos | 1 << SPI_CR1_MSTR_Pos);
    // Set NSS pin to output mode
    SPI1->CR2 |= SPI_CR2_SSOE;
    // Turn on SSM
    SPI1->CR1 |= SPI_CR1_SSM;
    // Enable SPI
    SPI1->CR1 |= SPI_CR1_SPE;
}


void displaySend(uint8_t command, uint8_t send) {
    digitalWrite(GPIOB, DISPLAY_DC, command);
    digitalWrite(GPIOA, DISPLAY_CS, 0);
    // SPI1->CR1 |= SPI_CR1_SPE;
    SPI1->DR = send;

    // Wait until message has been transmitted
    while(!(SPI1->SR & SPI_SR_RXNE));

    // Read to clear the RXNE flag
    volatile uint16_t reg = SPI1->DR;
    
    // SPI1->CR1 &= ~(SPI_CR1_SPE_Msk);
    digitalWrite(GPIOA, DISPLAY_CS, 1);
}

void writePixel(uint8_t x, uint8_t y, uint8_t val) {
    if(val) DISPLAYMEM[x + ((y / 8)*DISPLAY_WIDTH)] |= val << (y % 8);
    else DISPLAYMEM[x + ((y / 8)*DISPLAY_WIDTH)] &= ~(1 << (y % 8));
}

void writeCommand(uint8_t command) {
    switch (command) {
        case PUSH_IT:
            memcpy(DISPLAYMEM, Push_it_bitmap, sizeof(DISPLAYMEM));
            break;
        case WIRE_IT:
            memcpy(DISPLAYMEM, Wire_it_bitmap, sizeof(DISPLAYMEM));
            break;
        case HEAT_IT:
            memcpy(DISPLAYMEM, Heat_it_bitmap, sizeof(DISPLAYMEM));
            break;
        case SHAKE_IT:
            memcpy(DISPLAYMEM, Shake_it_bitmap, sizeof(DISPLAYMEM));
            break;
        case SHOUT_IT:
            memcpy(DISPLAYMEM, Shout_it_bitmap, sizeof(DISPLAYMEM));
            break;
        default:
            break;
    }
}

void writeMessage(char *message) {
    if(strcmp(message, "WELCOME") == 0) memcpy(DISPLAYMEM, Welcome_bitmap, sizeof(DISPLAYMEM));
    else if(strcmp(message, "START") == 0) memcpy(DISPLAYMEM, Start_bitmap, sizeof(DISPLAYMEM));
    else if(strcmp(message, "READY") == 0) memcpy(DISPLAYMEM, Ready_bitmap, sizeof(DISPLAYMEM));
    else if(strcmp(message, "GAME OVER") == 0) memcpy(DISPLAYMEM, Game_over_bitmap, sizeof(DISPLAYMEM));
}

void clearDisplay() {
    int x, y;
    for (x = 0; x < DISPLAY_WIDTH; ++x) {
        for (y = 0; y < DISPLAY_HEIGHT; ++y) {
            writePixel(x, y, 0);
        }
    }
    updateDisplay();
}

void updateDisplay() {
    // Set X address of RAM to 0
    displaySend(0, 0b10000000);
    // Set Y address of RAM to 0
    displaySend(0, 0b01000000);
    // We divide by 8 since we write 8 bits at a time
    for(int i = 0; i < DISPLAY_HEIGHT * DISPLAY_WIDTH / 8; ++i) {
        displaySend(1, DISPLAYMEM[i]);
    }
}

void writeDigit(int val,int x,int y) {
    switch(val) {
        case 0:
            DISPLAYMEM[x + ((y / 8)*DISPLAY_WIDTH)] = 0xFF;
            DISPLAYMEM[x+1 + ((y / 8)*DISPLAY_WIDTH)] = 0x81;
            DISPLAYMEM[x+2 + ((y / 8)*DISPLAY_WIDTH)] = 0xFF;
            break;
        case 1:
            DISPLAYMEM[x + ((y / 8)*DISPLAY_WIDTH)] = 0xFF;
            DISPLAYMEM[x+1 + ((y / 8)*DISPLAY_WIDTH)] = 0xFF;
            DISPLAYMEM[x+2 + ((y / 8)*DISPLAY_WIDTH)] = 0xFF;
            break;
        case 2:
            DISPLAYMEM[x + ((y / 8)*DISPLAY_WIDTH)] = 0xDF;
            DISPLAYMEM[x+1 + ((y / 8)*DISPLAY_WIDTH)] = 0xDB;
            DISPLAYMEM[x+2 + ((y / 8)*DISPLAY_WIDTH)] = 0xFB;
            break;
        case 3:
            DISPLAYMEM[x + ((y / 8)*DISPLAY_WIDTH)] = 0xDB;
            DISPLAYMEM[x+1 + ((y / 8)*DISPLAY_WIDTH)] = 0xDB;
            DISPLAYMEM[x+2 + ((y / 8)*DISPLAY_WIDTH)] = 0xFF;
            break;
        case 4:
            DISPLAYMEM[x + ((y / 8)*DISPLAY_WIDTH)] = 0xF8;
            DISPLAYMEM[x+1 + ((y / 8)*DISPLAY_WIDTH)] = 0x18;
            DISPLAYMEM[x+2 + ((y / 8)*DISPLAY_WIDTH)] = 0xFF;
            break;
        case 5:
            DISPLAYMEM[x + ((y / 8)*DISPLAY_WIDTH)] = 0xFB;
            DISPLAYMEM[x+1 + ((y / 8)*DISPLAY_WIDTH)] = 0xDB;
            DISPLAYMEM[x+2 + ((y / 8)*DISPLAY_WIDTH)] = 0xDF;
            break;
        case 6:
            DISPLAYMEM[x + ((y / 8)*DISPLAY_WIDTH)] = 0xFF;
            DISPLAYMEM[x+1 + ((y / 8)*DISPLAY_WIDTH)] = 0xDB;
            DISPLAYMEM[x+2 + ((y / 8)*DISPLAY_WIDTH)] = 0xDF;
            break;
        case 7:
            DISPLAYMEM[x + ((y / 8)*DISPLAY_WIDTH)] = 0xC0;
            DISPLAYMEM[x+1 + ((y / 8)*DISPLAY_WIDTH)] = 0xC0;
            DISPLAYMEM[x+2 + ((y / 8)*DISPLAY_WIDTH)] = 0xFF;
            break;
        case 8:
            DISPLAYMEM[x + ((y / 8)*DISPLAY_WIDTH)] = 0xFF;
            DISPLAYMEM[x+1 + ((y / 8)*DISPLAY_WIDTH)] = 0x99;
            DISPLAYMEM[x+2 + ((y / 8)*DISPLAY_WIDTH)] = 0xFF;
            break;
        case 9:
            DISPLAYMEM[x + ((y / 8)*DISPLAY_WIDTH)] = 0xF1;
            DISPLAYMEM[x+1 + ((y / 8)*DISPLAY_WIDTH)] = 0x91;
            DISPLAYMEM[x+2 + ((y / 8)*DISPLAY_WIDTH)] = 0xFF;
            break;
        default:
            break;
    }
}

void writeScore(int val, int x, int y) {
    displaySend(0, 0b10000000 + x);         // Set X address of Ram
    displaySend(0, 0b01000000 + y);         // Set Y address of Ram
    if(val % 100 >= 0) {
        writeDigit(val / 100, x, y);
        x += 4;
    }
    if(val % 10 >= 0) {
        writeDigit((val % 100) / 10, x, y);
        x += 4;
    }
    writeDigit(val % 10, x, y);
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