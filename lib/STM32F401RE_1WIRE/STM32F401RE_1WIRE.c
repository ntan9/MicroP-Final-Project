// STM32F401RE_1WIRE.c
#include "STM32F401RE_1WIRE.h"


void setupOneWire(void){
    initTIM(ONEWIRE_TIMx);
    digitalWrite(GPIOx, ONEWIRE_PIN, 1);        // Set PA6 to output
    pinMode(GPIOx, ONEWIRE_PIN, 1);
    GPIOx->OTYPER |= (1 << ONEWIRE_PIN);
    GPIOx->OSPEEDR |= (0b11 << ONEWIRE_PIN * 2);
}

uint8_t init(void){
    // Reset pulse
    digitalWrite(GPIOx, ONEWIRE_PIN, 0);
    delay_micros(ONEWIRE_TIMx, 480);
    digitalWrite(GPIOx, ONEWIRE_PIN, 1);


    // Read the bus every 60 microsec and delay for at least 480 us
    int time = 0;
    for (int i = 0; i < 4; ++i) {
        delay_micros(ONEWIRE_TIMx, 60);
        time += 60;
        if (digitalRead(GPIOx, ONEWIRE_PIN) == 0) {
            delay_micros(ONEWIRE_TIMx, 480 - time);
            return 1;
        }
    }
    return 0;
}

void write_bit(uint8_t val){
    // Set the bus low
    digitalWrite(GPIOx, ONEWIRE_PIN, 0);

    if (val == 0) {                             // Write-zero slot
        delay_micros(ONEWIRE_TIMx, 60);
        digitalWrite(GPIOx, ONEWIRE_PIN, 1);
    } else {                                    // Write-one slot
        delay_micros(ONEWIRE_TIMx, 5);
        digitalWrite(GPIOx, ONEWIRE_PIN, 1);
        delay_micros(ONEWIRE_TIMx, 60);
    }
}

uint8_t read_bit(void){
    // Initiate read process
    digitalWrite(GPIOx, ONEWIRE_PIN, 0);
    delay_micros(ONEWIRE_TIMx, 2);
    digitalWrite(GPIOx, ONEWIRE_PIN, 1);

    // Read the bus at about 12 us
    delay_micros(ONEWIRE_TIMx, 10);
    uint8_t output = digitalRead(GPIOx, ONEWIRE_PIN);

    // Delay for at least 60 us
    delay_micros(ONEWIRE_TIMx, 70);
    return output;
}

void write_byte(uint8_t val){
    // Write bit by bit starting from the LSB
    for (int i = 0; i < 8; i++) {
        uint8_t bit = val % 2;
        val = val >> 1;
        write_bit(bit);
    }
}

uint8_t read_byte(void){
    uint8_t output = 0;
    uint8_t shifter = 0;
    // Read the bit on the bus and add amount to a counter
    for (int i = 0; i < 8; i++) {
        uint8_t bit = read_bit();
        output +=  bit << shifter;
        shifter += 1;
    }
    return output;
}

void read_rom(uint8_t rom_code[8]){
    // Send initialization sequence
    while(init() == 0);
    // Send SKIP ROM command code
    write_byte(0xCC);
}

void skip_rom() {
    // Send initialization sequence
    while(init() == 0);
    // Send SKIP ROM command code
    write_byte(0xCC);
}

void read_scratchpad(uint8_t scratchpad[8]){
    // Send Read Scratchpad command
    write_byte(0xBE);

    // Read out scratchpad
    for (int i = 0; i < 8; i++) {
        uint8_t byte = read_byte();
        scratchpad[i] = byte;
    }

    volatile uint8_t crc = read_byte();
}

int getTemperature() {
    uint8_t scratchpad[8];

    // Init, skip rom, convert T
    skip_rom();
    write_byte(0x44);
    
    // Wait until conversion is done
    while(read_bit() == 0);

    // Init, skip rom, read scratchpad
    skip_rom();
    read_scratchpad(scratchpad);

    // Return the first two temperatures as one integer
    int temp = (scratchpad[1] << 8) + scratchpad[0];

    // Remove all decimals in the temperature
    temp = temp >> 4;

    return temp;
}

void initTempSensor() {
    // Init, skip rom, write scratchpad
    skip_rom();
    write_byte(0x4e);

    write_byte(0b01111111);     // Set Th to max
    write_byte(0b11111111);     // Set Tl to min
    write_byte(0b00011111);     // Configure sensor to 9 bit resolution
}