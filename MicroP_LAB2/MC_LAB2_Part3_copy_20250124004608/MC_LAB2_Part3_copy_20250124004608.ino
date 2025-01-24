// ENCE 3210 – Microprocessors 1 Lab 2 Part 3
// Sebastian Posada
// 1/24/2025
// Repeat Problem 7 from Lab 1 using interrupts.

// Defining pins connected to LEDs and buttons
#define RLED 4      
#define GLED 5
#define BLED 6      
#define BUTTON_EX 3  // Execution Button

// Arrays for operations
uint8_t countArray1[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};                
uint8_t countArray2[10] = {11, 12, 13, 14, 15, 16, 17, 18, 19, 20}; 
uint8_t sumArray[10];  // Array to store the results of operations

// Flags for execution and operation selection
volatile uint8_t execFlag = 0;  // Flag to indicate execution 


// ISR for execution button press
void execISR() {
    execFlag = 1; // Set execution flag for process to start
}

void setup() {
    // Set LED pins as outputs
    pinMode(RLED, OUTPUT);
    pinMode(GLED, OUTPUT);
    pinMode(BLED, OUTPUT);
    
    // Set button pins as input with internal pull-up resistors
    pinMode(BUTTON_OP, INPUT_PULLUP);
    pinMode(BUTTON_EX, INPUT_PULLUP);
    
    Serial.begin(9600); // Initialize serial communication at 9600 baud

    // Attach interrupts to buttons for handling presses asynchronously
    attachInterrupt(digitalPinToInterrupt(BUTTON_EX), execISR, FALLING); // Execution button ISR
}

void loop() {
    if (execFlag) { // Check if the execution flag is set 
        digitalWrite(RLED, HIGH);  // Turn on red LED to indicate processing started
        digitalWrite(GLED, LOW);   // Turn off green LED
        
        Serial.println("Array addition");
         // Perform element-wise addition of countArray1 and countArray2, storing results in sumArray
        for (int i = 0; i < 10; i++) {
            sumArray[i] = countArray1[i] + countArray2[i];
            Serial.print(sumArray[i]); // Print each result to the serial monitor
            Serial.println();
        }

        digitalWrite(RLED, LOW);  // Turn off red LED after processing
        digitalWrite(GLED, HIGH); // Turn on green LED to indicate completion
        
        execFlag = 0; // Reset execution flag after processing to wait for the next trigger
    }
}