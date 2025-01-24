// ENCE 3210 – Microprocessors 1 Lab 2 Part 2 
// Sebastian Posada
// 1/24/2025
// Repeat Problem 6 from Lab 1 using interrupts.


// Defining pins connected to LEDs and buttons
#define RLED 4
#define GLED 5
#define BLED 6
#define BUTTON 2

volatile int step = 0;               // Tracks the LED combination.
volatile bool buttonPressed = false; // Flag to track if the button was pressed

// Interrupt Service Routine for button press
void buttonISR() {
    if (!buttonPressed) { // Ensuring one press is counted at a time
        step++; 
        if (step > 5) step = 1; // Reset to step 1 after step 5
        buttonPressed = true;
    }
}

void setup() {
  // Setup LED's as outputs
    pinMode(RLED, OUTPUT);
    pinMode(GLED, OUTPUT);
    pinMode(BLED, OUTPUT);
    // Setup
    pinMode(BUTTON, INPUT_PULLUP);

    Serial.begin(9600);  // Setting up serial monitor at 9600 baud

    // Attach interrupt to the button 
    attachInterrupt(digitalPinToInterrupt(BUTTON), buttonISR, FALLING);
}

void loop() {
    if (buttonPressed) {
        // Reset all LEDs before applying the new step
        digitalWrite(RLED, LOW);
        digitalWrite(GLED, LOW);
        digitalWrite(BLED, LOW);

        Serial.print("Step: ");
        Serial.println(step);

        // Handling LED states based on step count
        switch (step) {
            case 1: Serial.println("Red ON"); digitalWrite(RLED, HIGH); break;
            case 2: Serial.println("Red + Green ON"); digitalWrite(RLED, HIGH); digitalWrite(GLED, HIGH); break;
            case 3: Serial.println("Green ON"); digitalWrite(GLED, HIGH); break;
            case 4: Serial.println("Green + Blue ON"); digitalWrite(GLED, HIGH); digitalWrite(BLED, HIGH); break;
            case 5: Serial.println("Blue ON"); digitalWrite(BLED, HIGH); break;
        }

        buttonPressed = false; // Reset flag to for next press
    }
}
