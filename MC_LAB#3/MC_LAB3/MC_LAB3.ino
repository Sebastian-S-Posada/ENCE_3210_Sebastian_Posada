// Micro Possessor Lab #3
// Sebastian Posada 1/31/2025
// Prompt: This assignment focuses on implementing multitasking using, 
// timers and interrupt service routines on an Arduino Uno.
// Defining LED and button pins

#define GLED 5      // Green LED on pin 5
#define RLED 4      // Red LED on pin 4
#define BUTTON 2    // Button on pin 2 

// Define global Variables
volatile uint16_t gCounter1 = 0;   // Counter for main task

void setup() {
    Serial.begin(9600);              // Start serial monitor

    pinMode(GLED, OUTPUT);           // Green LED as output
    pinMode(RLED, OUTPUT);           // Red LED as output
    pinMode(BUTTON, INPUT_PULLUP);   // Button as a pullup input

    cli();                           // Disable global interrupts 
    setupTimers();                   // Setup Timer1 for 1Hz
    sei();                           // Enable global interrupts

    // Attach interrupt on button press and release
    attachInterrupt(digitalPinToInterrupt(BUTTON), updateRedLED, CHANGE);
}

// Timer setup 
void setupTimers() {
    // Timer 1 for green LED at 1Hz and counter
    TCCR1A = 0;               
    TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10); // Prescaler 1024
    OCR1A = 15624;            // Match value for 1Hz
    TIMSK1 = (1 << OCIE1A);   // Enable Timer1 and match interrupt
}

// Timer 1 ISR for green LED and counter at 1Hz
ISR(TIMER1_COMPA_vect) {
    digitalWrite(GLED, !digitalRead(GLED)); // Toggle Green LED
    gCounter1++;  // Increment counter for main task
}

// Interrupt for button press and release to control red LED
void updateRedLED() {
    if (digitalRead(BUTTON) == LOW) {
        digitalWrite(RLED, HIGH);  // Turn on red LED when button is pressed
    } else {
        digitalWrite(RLED, LOW);   // Turn off red LED when button is released
    }
}

// Main Loop 
void loop() {
    static uint16_t lastCount = 0;

    // Print message every 3 seconds
    if (gCounter1 - lastCount >= 3) {
        Serial.println("Task counted every 3 seconds."); 
        lastCount = gCounter1;  // Update last counter check
    }
}
