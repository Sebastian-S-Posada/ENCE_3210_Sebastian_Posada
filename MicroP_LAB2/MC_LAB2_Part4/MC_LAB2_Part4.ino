// ENCE 3210 – Microprocessors 1 Lab 2 Part 4 
// Sebastian Posada
// 1/24/2025
// Write a C program such that the global integer array x with 10 elements will be filled initially. For this
// problem, fill it at the beginning of the code. When an interrupt comes from button 1 the ISR will be
// called. The ISR will calculate the global integer array y defined as 𝑦[𝑛] = 2 ∗ 𝑥[𝑛] − 𝑥[𝑛 − 1] where 𝑛 is
// the index for the array. In fact, this is a simple filtering operation working with interrupts. In the actual
// application, the interrupt should come from some other source. The array 𝑥 should also be filled by
// other module or a peripheral (such as ADC).

// Define the Button connected to pin 2
#define BUTTON 2  

volatile uint8_t interruptFlag = 0;  // Flag to indicate interrupt occurrence

int8_t x[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}; // Input array
int8_t y[10];  // Output array

// Interrupt Service Routine 
void handleInterrupt() {
    interruptFlag = 1;  // Set the flag to indicate the interrupt occurred
}

void setup() {
    Serial.begin(9600);  // Initialize serial communication
    pinMode(BUTTON, INPUT_PULLUP);  // Set button pin as input with internal pull-up resistor
    attachInterrupt(digitalPinToInterrupt(BUTTON), handleInterrupt, FALLING);  // Attach ISR to button press
}

void loop() {
    // Check if the interrupt has been triggered
    if (interruptFlag) {
        interruptFlag = 0;  // Reset the flag
        
        // Compute the filtered array using y[n] = 2*x[n] - x[n-1]
        y[0] = 2 * x[0];  // Special case for first element (no x[n-1] available)
        for (uint8_t n = 1; n < 10; n++) {
            y[n] = 2 * x[n] - x[n - 1];
        }

        // Print the results to the Serial Monitor
        Serial.println("Filtered array (y):");
        for (uint8_t n = 0; n < 10; n++) {
            Serial.println(y[n]);
        }
    }
}
