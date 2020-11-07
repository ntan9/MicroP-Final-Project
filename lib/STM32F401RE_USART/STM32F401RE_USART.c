// STM32F401RE_USART.c
// Source code for USART functions

#include "STM32F401RE_USART.h"
#include "STM32F401RE_GPIO.h"
#include "STM32F401RE_RCC.h"
#include "UARTRingBuffer.h"

USART_TypeDef * id2Port(uint32_t USART_ID){
    USART_TypeDef * USART;
    switch(USART_ID){
        case(USART1_ID) :
            USART = USART1;
            break;
        case(USART2_ID) :
            USART = USART2;
            break;
        default :
            USART = 0;
    }
    return USART;
}

USART_TypeDef * initUSART(uint8_t USART_ID, uint32_t baud_rate){
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;    // Enable GPIO port A

    USART_TypeDef * USART = id2Port(USART_ID);

    switch(USART_ID){
        case USART1_ID :
            RCC->APB2ENR |= (RCC_APB2ENR_USART1EN);   // Set USART1EN

            // Configure pin modes as ALT function
            pinMode(GPIOA, GPIO_PA9, GPIO_ALT); // TX
            pinMode(GPIOA, GPIO_PA10, GPIO_ALT); // RX

            GPIOA->AFR[1] |= (0b111 << GPIO_AFRH_AFSEL10_Pos | 0b111 << GPIO_AFRH_AFSEL9_Pos);
            break;
        case USART2_ID :
            RCC->APB1ENR |= (RCC_APB1ENR_USART2EN);       // Set USART1EN

            // Configure pin modes as ALT function
            pinMode(GPIOA, GPIO_PA2, GPIO_ALT); // TX
            pinMode(GPIOA, GPIO_PA3, GPIO_ALT); // RX

            // Configure pin modes as ALT function
            GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL2_Msk | GPIO_AFRL_AFSEL3_Msk);
            // Configure correct alternate functions (AF07)
            GPIOA->AFR[0] |= (0b0111 << GPIO_AFRL_AFSEL2_Pos | 0b0111 << GPIO_AFRL_AFSEL3_Pos);
            break;
    }

    USART->CR1 |= (USART_CR1_UE);     // Enable USART
    USART->CR1 &= ~(USART_CR1_M_Msk | USART_CR1_OVER8_Msk);     // M = 0 and set 16 times sampling freq
    USART->CR2 &= ~(USART_CR2_STOP_Msk);    // 0b00 corresponds to 1 stop bit

    // Set baud rate to 9.6 kbps
    // Tx/Rx baud = (f_CK)/(8*(2-OVER8)*USARTDIV) = Tx/Rx baud = (f_CK)/(16*USARTDIV)
    // f_CK = 84e6 Hz on APB2 (USART1) or 42e6 on APB1 (USART2)
    // USARTDIV = 546.875 should be in BRR
    // 546 = 0x0222
    // 0.875 = 7/8 = 0b1110
    // DIV_Mantissa = 0x222
    // DIV_Fraction = 0b111

    if(USART_ID == USART1_ID){
        USART->BRR |= ~(USART_BRR_DIV_Mantissa_Msk | USART_BRR_DIV_Fraction_Msk);

        if (baud_rate == 9600){
            USART->BRR |= (546 << USART_BRR_DIV_Mantissa_Pos | 0b1110 << USART_BRR_DIV_Fraction_Pos);
        } else {
            // Default to 115200
            USART->BRR |= (45 << USART_BRR_DIV_Mantissa_Pos | 0b1001 << USART_BRR_DIV_Fraction_Pos);
        }

    }
    else if (USART_ID == USART2_ID){
        USART->BRR |= ~(USART_BRR_DIV_Mantissa_Msk | USART_BRR_DIV_Fraction_Msk);
        if (baud_rate == 9600){
            USART->BRR |= (273 << USART_BRR_DIV_Mantissa_Pos | 0b0111 << USART_BRR_DIV_Fraction_Pos);
        } else {
            // Default to 115200
            USART->BRR |= (22 << USART_BRR_DIV_Mantissa_Pos | 0b1101 << USART_BRR_DIV_Fraction_Pos);
        }
    }

    // Enable tranmission and reception
    USART->CR1 |= (USART_CR1_TE | USART_CR1_RE);

    return USART;
}

void sendChar(USART_TypeDef * USART, uint8_t data){
    while(!(USART->SR & USART_SR_TXE));
    USART->DR = data & USART_DR_DR;
    while(!(USART->SR & USART_SR_TC));
}

void sendString(USART_TypeDef * USART, uint8_t * charArray){
    if (USART == USART1) {
        uint32_t i = 0;
        do{
            write_char_buffer(charArray[i]);
            i++;
        }
        while(charArray[i] != 0);
        USART->CR1 |= USART_CR1_TXEIE;          // Enable interupts for the buffer
    } else {
        uint32_t i = 0;
        do{
            sendChar(USART, charArray[i]);
            i++;
        }
        while(charArray[i] != 0);
    }
}
    

uint8_t readChar(USART_TypeDef * USART){
    if(is_data_available()){
        uint8_t data = read_char_buffer();
        return data;
    } else {
        return 0;
    }

}

void readString(USART_TypeDef * USART, uint8_t * charArray){
    uint32_t i = 0;
    do{
        charArray[i] = readChar(USART);
        i++;
    }
    while(is_data_available());
}