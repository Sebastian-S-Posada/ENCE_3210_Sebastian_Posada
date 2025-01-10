// Pin Definitions
const int redLED = A0;    // Red LED connected to analog pin A0
const int greenLED = A1;  // Green LED connected to analog pin A1
const int blueLED = A2;   // Blue LED connected to analog pin A2

// Array of letters (English alphabet)
char letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; // Contains all uppercase English letters
int rCount = 0, gCount = 0, bCount = 0; // Variables to count occurrences of 'R', 'G', 'B'

void setup() {
  // Initialize the LED pins as OUTPUT so they can control the LEDs
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(blueLED, OUTPUT);

  // Start Serial communication at a baud rate of 9600 for debugging
  Serial.begin(9600);

  // Seed the random number generator to make shuffling more random
  // Analog pin 0's noise is used as a random seed
  randomSeed(analogRead(0));

  // Process the array multiple times (e.g., 5 passes)
  for (int pass = 0; pass < 5; pass++) {
    randomizeArray(letters); // Shuffle the order of the letters in the array

    // Process each letter in the shuffled array
    for (int i = 0; i < sizeof(letters) - 1; i++) { // sizeof(letters) - 1 excludes the null terminator
      if (letters[i] == 'R') {
        rCount++;                   // Increment the counter for 'R'
        digitalWrite(redLED, HIGH); // Turn on the Red LED
        delay(200);                 // Keep it on for 200ms
        digitalWrite(redLED, LOW);  // Turn off the Red LED
        Serial.println("R detected!"); // Log detection of 'R'
      } else if (letters[i] == 'G') {
        gCount++;                   // Increment the counter for 'G'
        digitalWrite(greenLED, HIGH); // Turn on the Green LED
        delay(200);                 // Keep it on for 200ms
        digitalWrite(greenLED, LOW); // Turn off the Green LED
        Serial.println("G detected!"); // Log detection of 'G'
      } else if (letters[i] == 'B') {
        bCount++;                   // Increment the counter for 'B'
        digitalWrite(blueLED, HIGH); // Turn on the Blue LED
        delay(200);                 // Keep it on for 200ms
        digitalWrite(blueLED, LOW); // Turn off the Blue LED
        Serial.println("B detected!"); // Log detection of 'B'
      }
    }

    // Print results for the current pass
    Serial.print("Pass ");
    Serial.println(pass + 1);       // Display the current pass number
    Serial.print("Occurrences of R: ");
    Serial.println(rCount);         // Display the number of times 'R' was found
    Serial.print("Occurrences of G: ");
    Serial.println(gCount);         // Display the number of times 'G' was found
    Serial.print("Occurrences of B: ");
    Serial.println(bCount);         // Display the number of times 'B' was found
    Serial.println("--------------------------"); // Separator for clarity
  }
}

void loop() {
  // No action needed in the loop since everything happens in setup
}

// Function to randomize the order of an array using the Fisher-Yates Shuffle
void randomizeArray(char arr[]) {
  int n = sizeof(letters) - 1; // Calculate the number of elements in the array (excluding null terminator)
  for (int i = n - 1; i > 0; i--) { // Iterate backward through the array
    int j = random(0, i + 1); // Generate a random index from 0 to i
    // Swap arr[i] and arr[j]
    char temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
  }
}
