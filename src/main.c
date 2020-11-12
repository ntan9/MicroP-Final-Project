#include "main.h"

uint8_t gameStarted;                // Tracks if the game has started or not
uint8_t gameOver;                   // 
uint32_t gameDelay;                 // Tracks the amount of game will delay after each turn
uint32_t score;                     // Tracks score of player
uint8_t task;                       // Stores the task given to the player
uint8_t input;                      // Stores the user's registered input
uint8_t ambientTemp;                // Stores the ambient room temperature when the game starts
int numCommands;                    // Used to generate random int of range [0, numCommands)

// All commands that the game will support
uint8_t commands[] = {PUSH_IT, SHOUT_IT, WIRE_IT, SHAKE_IT, HEAT_IT};     

/**
ko * Main program.
 */
int main(void) {
    uint8_t msg[64];
    uint16_t adc_val;
    configureFlash();
    configureClock();

    // Enable GPIOA, GPIOB, GPIOC
    RCC->AHB1ENR  |= (RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN);     
    // Enable TIM2 and I2C1
    RCC->APB1ENR  |= (RCC_APB1ENR_TIM2EN | RCC_APB1ENR_I2C1EN);
    // Enable SYSCFG clock domain
    RCC->APB2ENR  |= (RCC_APB2ENR_SYSCFGEN);    

    initUSART(USART2_ID, 115200);               // Initialize USART2 to print to terminal
    initTIM(DELAY_TIM);                              // Initialize TIM2
    initADC(ADC1, GPIOA, 0, 6);                 // Initialize ADC to pin A0
    initPushButton();

    pinMode(GPIOA, 0, GPIO_ANALOG);             // PA0 is input for ADC
    pinMode(GPIOC, 13, GPIO_INPUT);             // PC13 is Nucleo Push Button

    ///////////////////
    // I2C1 Set up
    //////////////////

    // Set PB6 and PB7 to alternate function 4 and to open drain configuration
    GPIOB->AFR[0] |= (4 << GPIO_AFRL_AFSEL6_Pos | 4 << GPIO_AFRL_AFSEL7_Pos);
    pinMode(GPIOB, 6, 2);
    pinMode(GPIOB, 7, 2);
    GPIOB->OTYPER |= (1 << GPIO_OTYPER_OT6_Pos | 1 << GPIO_OTYPER_OT7_Pos);

    initI2C(I2C1);
    setUpAccelerometer(I2C1);



    // Enable interrupts globally
    __enable_irq();

    gameStarted = 0;
    input = 0;

    while (1) {
        delay_millis(DELAY_TIM, MESSAGE_DELAY);
        sendString(USART2, "Push button to begin!\n\r");
        // adc_val = read_ADC(ADC1);
        // sprintf(msg, "ADC Value: %d\n\r", adc_val);
        // sendString(USART2, msg);



        // Wait for user to start the game
        while(!gameStarted){
            /* 
             * For some reason, without the delay line the interrupt doesn't trigger
             * I think this has to do with the compiler optimizing parts out
             */
            delay_millis(DELAY_TIM, MESSAGE_DELAY);

        }
        // Initialize a random seed
        srand(time(NULL));          
        
        // Resets game elements
        score = 0;
        gameOver = 0;
        gameDelay = GAME_DELAY;
        ambientTemp = 0;
        numCommands = NUM_COMMANDS;

        sendString(USART2, "Ready?\n\r");
        delay_millis(DELAY_TIM, MESSAGE_DELAY);
        sendString(USART2, "Begin!\n\r");

        // Main game functionality
        while(1) {
            // Clear user input and selects a random command for task
            input = 0;
            task = commands[rand() % numCommands];
            switch (task)
            {
            case PUSH_IT:
                sendString(USART2, "Push It!\n\r");
                waitForInput(gameDelay);
                if(task != input) goto game_over;
                ++score;
                input = 0;
                gameDelay += GAME_DELAY_CHANGE;
                break;
            case SHOUT_IT:
                sendString(USART2, "Shout It!\n\r");
                break;
            case WIRE_IT:
                sendString(USART2, "Wire It!\n\r");

                // add modified delay function to check if the accelerometer has been shook or not

                break;
            case HEAT_IT:
                sendString(USART2, "Heat It!\n\r");
                --numCommands;
                break;
            case SHAKE_IT:
                sendString(USART2, "Shake It!\n\r");
                break;
            default:
                break;
            }
        }
        
game_over:
        // Game Over Handler
        gameStarted = 0;
        sendString(USART2, "You Failed!\n\r");
        sprintf(msg, "Your score was %d!\n\r", score);
        sendString(USART2, msg);
        delay_millis(DELAY_TIM, MESSAGE_DELAY);
    }
}


void initPushButton() {
    // Set EXTICR4 for PC13
    SYSCFG->EXTICR[3] &= ~(SYSCFG_EXTICR4_EXTI13_Msk);
    SYSCFG->EXTICR[3] |= (0b0010 << SYSCFG_EXTICR4_EXTI13_Pos);

    EXTI->IMR |= EXTI_IMR_IM13;             // Configure IM13 mask bit
    EXTI->RTSR &= ~(EXTI_RTSR_TR13_Msk);    // Disable rising edge trigger
    EXTI->FTSR |= EXTI_FTSR_TR13;           // Enable falling edge trigger
    __NVIC_EnableIRQ(40);                   // Enable External Line[15:10] Interrupts
}

// Waits for gameDelay time to get input from user
void waitForInput(uint32_t gameDelay) {
    // Placeholder for current temperature
    uint8_t currTemp = 0;

    if (currTemp <= ambientTemp + 1) {
        numCommands = NUM_COMMANDS;
    }

    while (gameDelay > 0) {
        // Poll the temperature sensor and accelerometer for any user input
        uint8_t temp = HEAT_IT * (0 > (currTemp + 2));
        uint8_t shake = SHAKE_IT * detectMotion(I2C1);
        
        // If either temp or shake is triggered, update input if input is 0
        if ((temp || shake) || input) {
            if (input) break;
            
            // Choose shake if both are non-zero, otherwise choose the non-zero var as input
            input = max(temp, shake);
            break;
        }

        // Delay for 5 milliseconds and poll again
        gameDelay -= 5;
        delay_millis(DELAY_TIM, 5);
    }
}

/*
 * Interrupts
 */

// Push button interrupt handler
void EXTI15_10_IRQHandler(void){
    // Check that the button EXTI_13 was what triggered our interrupt
    if (EXTI->PR & (1 << PUSH_BUTTON)){
        // Clear Interrupt
        EXTI->PR |= (1 << PUSH_BUTTON);
        if(!gameStarted) {
            gameStarted = 1;
        }
        else {
            input = PUSH_IT;
            DELAY_TIM->EGR |= TIM_EGR_UG;    // Write bit to clear the delay timer
        }
        // sendString(USART2, "Push it!\n\r");
    }
}

