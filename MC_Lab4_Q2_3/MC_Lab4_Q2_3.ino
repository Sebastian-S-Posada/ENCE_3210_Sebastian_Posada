/****************************************************
* MC_Lab4_Q2and3.ino                                *
* Sebastian Posada,                                 *
* 2/28/2025 8AM,                                    *
*                                                   *
*Used Chat GPT to help with debugging and code Flow *
****************************************************/

// Include libraries 
#include <Wire.h>                    // I2C  library
#include <Adafruit_GFX.h>            // Graphics library 
#include <Adafruit_SSD1306.h>        // OLED display library

// Display settings
#define SCREEN_WIDTH 128             // Pixle width
#define SCREEN_HEIGHT 64             // Pixel Hight
#define OLED_RESET -1                // OLED reset 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); // Initialize OLED display

// Pin Definitions
#define solarVoltagePin A0      // Solar panel voltage sensing
#define batteryVoltagePin A1    // Battery voltage sensing
#define switchPin 7             // Switching button

// Voltage Thresholds
const float solarThreshold = 2.5;    // Threshold for solar panel voltage to supply power
const float batteryThresholdOffset = 0.1; // Small offset to prevent rapid switching 

// Voltage Divider Ratio
const float voltageDividerRatio = 5.0; 

/**************************
*********FUNCTIONS*********
**************************/

// Function to read and calculate the actual voltage from an analog pin
float readVoltage(int pin) {
    int sensorValue = analogRead(pin);                  // Read the analog value
    float voltage = (sensorValue * 5.0 / 1023.0) * voltageDividerRatio; // Convert to real voltage
    return voltage;                                     // Return the voltage
}

// Function to display power routing status on the OLED display
void displayStatus(String status) {
    display.clearDisplay();                             // Clear display
    display.setTextSize(1);                             // Set text size to 1FO
    display.setTextColor(WHITE);                        // Set text color to white
    display.setCursor(0, 0);                            // Set cursor position for title
    display.println("Power Routing:");                  // Display title
    display.setCursor(0, 30);                           // Set cursor position for status
    display.println(status);                            // Display the power routing status
    display.display();                                  // Update display
}

/**************************
*********SETUP*************
**************************/
void setup() {
    // Initialize Serial Monitor
    Serial.begin(9600);
    // Initialize display
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {   
        Serial.println(F("Display failed"));
        while (true);                                  
    }
    display.display();                                 // Turn on the display
    delay(1000);                                       // Short delay to stabilize display
    display.clearDisplay();                            // Clear display after initialization
    // Setup the switching pin
    pinMode(switchPin, OUTPUT);                         // Set the switching pin as output
    digitalWrite(switchPin, LOW);                       // Set defult state as off
    // Start up message
    Serial.println("Solar Charge Controller Started!!");
}
/**************************
*********MAIN-LOOP*********
**************************/
void loop() {
    // Read voltages from solar panel and battery
    float solarVoltage = readVoltage(solarVoltagePin);  // Solar panel voltage
    float batteryVoltage = readVoltage(batteryVoltagePin); // Battery voltage
    // Display the voltages on Serial Monitor 
    Serial.print("Solar Voltage: ");
    Serial.print(solarVoltage);
    Serial.print("V, Battery Voltage: ");
    Serial.print(batteryVoltage);
    Serial.println("V");
    // Switch power based on solar panel voltage
    if (solarVoltage > solarThreshold) {
        digitalWrite(switchPin, LOW);                   // Use Solar Panel as power source
        displayStatus("Solar Power");                   // Display "Solar Power"
    } 
    else if (solarVoltage <= solarThreshold) {
        digitalWrite(switchPin, HIGH);                  // Use Battery as power source
        displayStatus("Battery Power");                 // Display "Battery Power" 
    }
    // Logic to charge the battery when conditions are met
    if (batteryVoltage < (solarVoltage - batteryThresholdOffset)) {
        digitalWrite(switchPin, LOW);                   // Connect battery to solar panel for charging
        displayStatus("Charging");                      // Display "Charging"
    }
    delay(1000);                                        
}
