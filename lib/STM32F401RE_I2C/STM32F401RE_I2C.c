#include "STM32F401RE_I2C.h"

void initI2C(I2C_TypeDef * I2Cx) {

    // Set FREQ to 10 MHz
    I2Cx->CR2 |= 10 << I2C_CR2_FREQ_Pos;

    // Configure Clock control register to Fast Mode with 400 kHz as SCL
    I2Cx->CCR |= (1 << I2C_CCR_FS_Pos | 120 << I2C_CCR_CCR_Pos);

    // Using max rise time to be 1000 ns, configure TRISE to be 10 + 1
    I2Cx->TRISE &= ~(I2C_TRISE_TRISE_Msk);
    I2Cx->TRISE |= 11 << I2C_TRISE_TRISE_Pos;

    // Enable I2C peripheral
    I2Cx->CR1 |= (1 << I2C_CR1_PE_Pos);
}

void writeI2C(I2C_TypeDef * I2Cx, uint8_t reg, uint8_t data) {
    // Set START bit
    I2Cx->CR1 |= 1 << I2C_CR1_START_Pos;

    // Wait until SB bit is set by hardware
    while (!(I2Cx->SR1 & I2C_SR1_SB_Msk));

    // Send device address with LSB as 0, indicating a write
    uint8_t address = DEVICE << 1;
    I2Cx->DR = address;

    // Wait until ADDR bit is set
    while (!(I2Cx->SR1 & I2C_SR1_ADDR_Msk));
    // Read SR2 to check if bus is in transmission mode
    if (!(I2Cx->SR2 & I2C_SR2_TRA)) {   
        return writeI2C(I2Cx, reg, data);    // If TRA indicates receiver, start write over
    }

    // Push device register address into DR register
    I2Cx->DR = reg;

    // Wait until data register is empty
    while (!(I2Cx->SR1 & I2C_SR1_TXE_Msk));

    // Push data to register
    I2Cx->DR = data;

    // Wait until data register is empty
    while (!(I2Cx->SR1 & I2C_SR1_TXE_Msk));

    // Set the STOP bit
    I2Cx->CR1 |= 1 << I2C_CR1_STOP_Pos;
}

uint8_t readI2C(I2C_TypeDef * I2Cx, uint8_t reg) {
    // Set START bit
    I2Cx->CR1 |= 1 << I2C_CR1_START_Pos;

    // Wait until SB bit is set by hardware
    while (!(I2Cx->SR1 & I2C_SR1_SB_Msk));

    // Send device address with LSB as 0, indicating a write
    uint8_t address = DEVICE << 1;
    I2Cx->DR = address;

    // Wait until ADDR bit is set
    while (!(I2Cx->SR1 & I2C_SR1_ADDR_Msk));
    // Read SR2 to check if bus is in transmission mode
    if (!(I2Cx->SR2 & I2C_SR2_TRA)) {
        return readI2C(I2Cx, reg);    // If TRA indicates receiver, start write over
    }

    // Push device register address into DR register
    I2Cx->DR = reg;

    // Wait until data register is empty
    while (!(I2Cx->SR1 & I2C_SR1_TXE_Msk));

    //////////////////////////////////////////////////////////////

    // Initiate another start pulse
    I2Cx->CR1 |= 1 << I2C_CR1_START_Pos;

    // Wait until SB bit is set by hardware
    while (!(I2Cx->SR1 & I2C_SR1_SB_Msk));

    // Send device address with LSB as 1, indicating a read
    address = DEVICE << 1;
    I2Cx->DR = address + 1;

    // Wait until ADDR bit is set
    while (!(I2Cx->SR1 & I2C_SR1_ADDR_Msk));
    // Read SR2 to check if bus is in receiver mode
    if (I2Cx->SR2 & I2C_SR2_TRA) {
        return readI2C(I2Cx, reg);    // If TRA indicates transmitter, start write over
    }

    // Generates nonacknowledge pulse and stop pulse
    I2Cx->CR1 |= 1 << I2C_CR1_STOP_Pos;

    // Wait until full byte received
    while (!(I2Cx->SR1 & I2C_SR1_RXNE_Msk));
    
    volatile uint8_t result = I2Cx->DR & I2C_DR_DR_Msk;

    return result;
}

void setUpAccelerometer(I2C_TypeDef * I2Cx) {
    writeI2C(I2Cx, 0x2a, 0x18);     // Place device in Standby mode at 100Hz ODR
    writeI2C(I2Cx, 0x15, 0xf8);     // Configure motion detection with ELE latch
    writeI2C(I2Cx, 0x17, 0x10);     // Set threshold value to be > 1g
    writeI2C(I2Cx, 0x18, 0x0a);     // Set debounce counter to 10 counts
    writeI2C(I2Cx, 0x2a, 0x19);     // Put device in Active mode
}

int detectMotion(I2C_TypeDef * I2Cx) {
    volatile uint8_t reg = readI2C(I2Cx, 0x16);
    return reg >> 7;
}