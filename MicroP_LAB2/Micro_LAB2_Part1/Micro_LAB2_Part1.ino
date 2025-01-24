// ENCE 3210 – Microprocessors 1 Lab 2 Part 1
// Sebastian Posada
// 1/24/2025
// Part 1: Write a C program to count the difference between the number of times two different buttons are
//  pressed.
//  a) The button pressing operation should be defined in an ISR.
//  b) Observe the count value from the watch window.
//  c) If button 1 is pressed more than button 2, then turn on the red color LED. If the button pressed
//  numbers is equal, then turn on the blue color LED. Otherwise, turn on the green color LED. If the
//  button press numbers are equal, then turn on the blue color LED.

// Defining pins connected to LEDs and buttons
#define BUTTON_1 2
#define BUTTON_2 3
#define RLED 4      
#define GLED 5      
#define BLED 6

// define the counter as a volatile int.
volatile uint8_t button1Count = 0;
volatile uint8_t button2Count = 0; 

// Interrupt Service Routine for Button 1
void BUTTON_1ISR() {
  button1Count++;
}

// Interrupt Service Routine for Button 2
void BUTTON_2ISR() {
  button2Count++;
}

// Setup LED's as outputs and buttons and inputs. 
// Setup serial monitor as 9600 baud.
void setup() {
  pinMode(RLED, OUTPUT);
  pinMode(GLED, OUTPUT);
  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);
  Serial.begin(9600);

  attachInterrupt(digitalPinToInterrupt(2), BUTTON_1ISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(3), BUTTON_2ISR, FALLING);
}


void loop() {
  // Print button counts for debugging
  Serial.print("Button 1: ");
  Serial.print(button1Count);
  Serial.print(" | Button 2: ");
  Serial.println(button2Count);

  // Logic to control LEDs based on button presses
  if (button1Count > button2Count) {
    digitalWrite(RLED, HIGH);   // Turn on Red LED
    digitalWrite(GLED, LOW);
    digitalWrite(BLED, LOW);
  } 
  else if (button1Count < button2Count) {
    digitalWrite(GLED, HIGH);   // Turn on Green LED
    digitalWrite(RLED, LOW);
    digitalWrite(BLED, LOW);
  } 
  else { // If counts are equal
    digitalWrite(BLED, HIGH);   // Turn on Blue LED
    digitalWrite(RLED, LOW);
    digitalWrite(GLED, LOW);
  }

  delay(500); 
}