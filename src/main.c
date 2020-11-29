// main.c
#include "main.h"

#define VOP_VAL 80

uint8_t gameStarted;                // Tracks if the game has started or not
uint8_t gameOver;                   // Tracks Game over state
uint32_t gameDelay;                 // Tracks the amount of game will delay after each turn
uint32_t score;                     // Tracks score of player
uint8_t task;                       // Stores the task given to the player
uint8_t input;                      // Stores the user's registered input

uint16_t ambientTemp;               // Stores the ambient room temperature when the game starts
uint16_t currTemp;		    		// Stores current temperature when "Heat It" is issued


// All commands that the game will support
uint8_t commands[] = {PUSH_IT, SHOUT_IT, WIRE_IT, SHAKE_IT, HEAT_IT};     


void initPushButton() {
	// Set EXTICR4 for PC13
	SYSCFG->EXTICR[3] &= ~(SYSCFG_EXTICR4_EXTI13_Msk);
	SYSCFG->EXTICR[3] |= (0b0010 << SYSCFG_EXTICR4_EXTI13_Pos);

	EXTI->IMR |= EXTI_IMR_IM13;             // Configure IM13 mask bit
	EXTI->RTSR &= ~(EXTI_RTSR_TR13_Msk);    // Disable rising edge trigger
	EXTI->FTSR |= EXTI_FTSR_TR13;           // Enable falling edge trigger
	__NVIC_EnableIRQ(EXTI15_10_IRQn);       // Enable External Line[15:10] Interrupts
}

void initWireIt() {
	pinMode(GPIOA, WIRE_IT_PIN, GPIO_INPUT);

	// Set EXTICR2 for PA5
	SYSCFG->EXTICR[1] &= ~(SYSCFG_EXTICR2_EXTI5_Msk);

	EXTI->IMR |= EXTI_IMR_IM5;		// Configure IM5 mask bit
	EXTI->RTSR |= EXTI_RTSR_TR5_Msk;	// Enable rising edge trigger
	EXTI->FTSR &= ~(EXTI_FTSR_TR5);		// Disable falling edge trigger
	__NVIC_EnableIRQ(EXTI9_5_IRQn);		// Enable External Line[9:5] Interrupts
}

void initADCInterrupt() {
    __NVIC_EnableIRQ(ADC_IRQn);
}

// Waits for gameDelay time to get input from user
void waitForInput(uint32_t gameDelay) {
	while (gameDelay > 0 && input == 0) {
		// Initializes inputs from user
		uint8_t temp = 0;
		uint8_t shake = SHAKE_IT * detectMotion(I2C1);

		// Reads for any input
		read_ADC(ADC1);

		// Read temperature if needed
		if (task == HEAT_IT) {
			temp = (getTemperature() > (currTemp + 2));
			temp *= HEAT_IT;
		}
	
		// If either temp or shake is triggered, update input if input is 0
		if ((temp || shake) || input) {
			if (input) return;
			
			// Choose shake if both are non-zero, otherwise choose the non-zero var as input
			input = max(temp, shake);
			return;
		}

		// Delay for 5 milliseconds and poll again
		gameDelay -= 5;
		delay_millis(DELAY_TIM, 5);
	}
}

// Initialize the LCD Display
void displayInit(void) {
	// Power on the display
	digitalWrite(GPIOA, 3, 1);

	// Send reset pulse
	digitalWrite(GPIOB, DISPLAY_RESET, 0);
	delay_micros(TIM2, 100);
	digitalWrite(GPIOB, DISPLAY_RESET, 1);
	
	digitalWrite(GPIOA, DISPLAY_CS, 1);

	// Set Power Down = 0, V to 0, and H to 1 (extended instruction set)
	displaySend(0, 0b00100001);
	// Set Vop
	displaySend(0, 0b10000000 + VOP_VAL);
	// Set Power Down = 0, V to 0 (horizontal addressing), and H to 0 (normal instruction)
	displaySend(0, 0b00100000);
	// Set display configuration to normal mode
	displaySend(0, 0b00001100);
}

/**
 * Main program.
 */
int main(void) {
	uint8_t msg[64];
	uint16_t adc_val;
	configureFlash();
	configureClock();

	/////////////////////////
	// Enable Clock Domains
	/////////////////////////

	// Enable GPIOA, GPIOB, GPIOC
	RCC->AHB1ENR  |= (RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN);     
	// Enable TIM2, TIM5, and I2C1
	RCC->APB1ENR  |= (RCC_APB1ENR_TIM2EN | RCC_APB1ENR_TIM5EN |RCC_APB1ENR_I2C1EN);
	// Enable SYSCFG clock domain and SPI1
	RCC->APB2ENR  |= (RCC_APB2ENR_SYSCFGEN | RCC_APB2ENR_SPI1EN);

	/////////////////////////////////
	// Init TIM2/TIM5
	/////////////////////////////////

	initTIM(DELAY_TIM);                         // Initialize TIM2
	configMusicTIM(SOUND_TIM);					// Initialize TIM5

	// Enable channel 1 for TIM5
	pinMode(GPIOA, 0, GPIO_ALT);
	GPIOA->AFR[0] |= (2 << GPIO_AFRL_AFSEL0_Pos);

	////////////////////
	// ADC Set Up
	///////////////////

	initADC(ADC1, GPIOA, 1, 7);                 // Initialize ADC to pin A1
	pinMode(GPIOA, 1, GPIO_ANALOG);             // PA1 is input for ADC
	initADCInterrupt();

	/////////////////////
	// Push Button Set Up
	/////////////////////

	initPushButton();
	pinMode(GPIOC, PUSH_BUTTON, GPIO_INPUT);             // PC13 is Nucleo Push Button

	///////////////////
	// I2C1 Set up
	//////////////////

	// Set PB6 and PB7 to alternate function 4 and to open drain configuration
	GPIOB->AFR[0] |= (4 << GPIO_AFRL_AFSEL6_Pos | 4 << GPIO_AFRL_AFSEL7_Pos);
	pinMode(GPIOB, 6, GPIO_ALT);
	pinMode(GPIOB, 7, GPIO_ALT);
	GPIOB->OTYPER |= (1 << GPIO_OTYPER_OT6_Pos | 1 << GPIO_OTYPER_OT7_Pos);

	initI2C(I2C1);
	setUpAccelerometer(I2C1);

	//////////////////////
	// Temp Sensor Set Up
	//////////////////////

	setupOneWire();		// Sets up OneWire communication over PA6
	initTempSensor();	// Configure temp sensor to be 9 bits resolution

	//////////////////////
	// Display SPI Set Up
	//////////////////////

	spiInit(4, 1, 1);
	digitalWrite(GPIOA, 3, GPIO_OUTPUT);
	delay_millis(TIM2, MESSAGE_DELAY);
	displayInit();

	//////////////////////
	// Wire it setup
	//////////////////////

	initWireIt();


	// Enable interrupts globally and initialize random seed
	__enable_irq();	
	srand(time(NULL));

	while (1) {
		// Reset game state
		gameStarted = 0;
		delay_millis(DELAY_TIM, MESSAGE_DELAY);
		writeMessage("WELCOME");
		updateDisplay();

		// Wait for user to start the game
		while(!gameStarted){
			delay_millis(DELAY_TIM, MESSAGE_DELAY);
		}
		
		// Initialize a new random seed
		srand(read_ADC(ADC1)); 

		// Reinitializes game elements
		score = 0;
		gameOver = 0;
		gameDelay = GAME_DELAY;
		ambientTemp = getTemperature();
		detectMotion(I2C1);

		// Send visual and audio cue that game has started
		playMusic(START);
		writeMessage("READY");
		updateDisplay();
		delay_millis(DELAY_TIM, MESSAGE_DELAY);
		writeMessage("START");
		updateDisplay();

		// Main game functionality
		while(1) {

			// Only add "Heat It" to pool of commands if temp is low enough
			currTemp = getTemperature();
			if (currTemp <= ambientTemp + 2) {
				// task = commands[score % NUM_COMMANDS]	// Demo code
				task = commands[rand() % NUM_COMMANDS];
			} else {
				// task = commands[score % NUM_COMMANDS]	// Demo code
				task = commands[rand() % (NUM_COMMANDS - 1)];
			}

			// Write command to screen and play corresponding music
			writeCommand(task);
			updateDisplay();
			playMusic(task);

			__enable_irq();

			// Clear user input and selects a random command for task
			input = 0;
			detectMotion(I2C1);

			// Handles different tasks
			switch (task)
			{
			case SHOUT_IT:
				calibrate_ADC(ADC1);
				waitForInput(gameDelay);
				break;
			case HEAT_IT:
				// Decrease delay since heat it takes longer
				waitForInput(gameDelay/4);
				break;
			default:
				waitForInput(gameDelay);
				break;
			}

			// Processes user input
			if(task != input) goto game_over;
			__disable_irq();

			// Play victory tone, update game state
			playMusic(SUCCESS);
			++score;
			if(gameDelay > MIN_GAME_DELAY) gameDelay += GAME_DELAY_CHANGE;
			
			// Clear flag in motion sensor
			delay_millis(DELAY_TIM, MESSAGE_DELAY);
		}
		
game_over:
		// Game Over Handler
		writeMessage("GAME OVER");
		writeScore(score, DISPLAY_WIDTH - 12, DISPLAY_HEIGHT - 8);
		updateDisplay();

		playMusic(GAME_OVER);
		delay_millis(DELAY_TIM, MESSAGE_DELAY);
	}
}

/*
 * Interrupts
 */

// Push button interrupt handler
void EXTI15_10_IRQHandler(void) {
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
	}
}

// Wire it interrupt handler
void EXTI9_5_IRQHandler(void) {
	// Check that the button EXTI_5 was what triggered our interrupt
	if(EXTI->PR & (1 << WIRE_IT_PIN)) {
		// Clear Interrupt
		EXTI->PR |= (1 << WIRE_IT_PIN);
		input = WIRE_IT;
		DELAY_TIM->EGR |= TIM_EGR_UG;		// Write bit to clear the delay timer
	}
}

// ADC interrupt handler
void ADC_IRQHandler(void) {
	if(ADC1->SR & ADC_SR_AWD) {
		// Disable watchdog interrupt enable
    		ADC1->CR1 &= ~ADC_CR1_AWDIE;
		// Clear interrupt
		ADC1->SR &= ~(ADC_SR_AWD);

		input = SHOUT_IT;
		DELAY_TIM->EGR |= TIM_EGR_UG;	// Write bit to clear the delay timer
	}
}