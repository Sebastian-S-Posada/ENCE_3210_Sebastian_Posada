/****************************************************
* MC_Lab4_Q4.ino                                    *
* Sebastian Posada,                                 *
* 2/28/2025 8AM,                                    *
*                                                   *
*Used Chat GPT to help with debugging and code Flow *
****************************************************/


// Pin Definitions
#define tempSensorPin A0           // Temperature sensor
#define fanPin 9                   // Fan control
#define buttonOnOff 2              // Fan On/Off button
#define buttonSensitivity 3        // Sensitivity button

// Variables
int sensitivityLevel = 1;          // Fan speed sensitivity level 
char fanState = '0';               // Fan state 0=OFF, 1=ON
int tempSamples = 100;             // Number of samples for temperature averaging
int readDelay = 50;                // Delay between read temperature sample

/********************************
**************SETUP**************
********************************/

void setup() {
  pinMode(tempSensorPin, INPUT);       
  pinMode(fanPin, OUTPUT);             
  pinMode(buttonOnOff, INPUT); 
  pinMode(buttonSensitivity, INPUT); 
  Serial.begin(9600);                  

  // Set Timer for PWM
  TCCR1A = (1 << WGM10) | (1 << COM1A1);  
  TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10); // Prescaler 1024
}


/********************************
***********MAIN-LOOP*************
********************************/

// Loop Function - Repeats continuously after setup()
void loop() {
  // Button On/Off Toggle
  if (digitalRead(buttonOnOff) == LOW) {   // Check if On/Off button is pressed
    delay(100);                          
    if (digitalRead(buttonOnOff) == LOW) { // Check again
      fanState = (fanState == '0') ? '1' : '0';  // Toggle fan state
    }
    while (digitalRead(buttonOnOff) == LOW); // Wait for button
  }

  // Change Sensitivity
  if (digitalRead(buttonSensitivity) == LOW) {  // Check if button is pressed
    delay(100);                                
    if (digitalRead(buttonSensitivity) == LOW) {  // Check again to
      sensitivityLevel++;                       // Increase sensitivity
      if (sensitivityLevel > 3) sensitivityLevel = 1;  // Cycle back to 1 if > 3
    }
    while (digitalRead(buttonSensitivity) == LOW);  // Wait for button
  }

  // Calculate Average Temperature
  float avgTemp = readTemperature();   // Read average temperature
  // Display on Serial Monitor
  Serial.print("Average Temperature: ");
  Serial.print(avgTemp);              
  Serial.print(" C | Sensitivity Level: ");
  Serial.println(sensitivityLevel);    

  // Control Fan Speed
  if (fanState == '1') {                // Check if fan is ON
    int pwmValue = calculateFanSpeed(avgTemp, sensitivityLevel);  // Get PWM value for fan speed
    analogWrite(fanPin, pwmValue);      // Set PWM value to fan
  } else {
    analogWrite(fanPin, 0);             // Turn off fan
  }
  delay(500);                     
}

/********************************
**********FUNCTIONS**************
********************************/

// Function to Read Average Temperature
float readTemperature() {
  long sum = 0;                            // Initialize sum for averaging
  for (int i = 0; i < tempSamples; i++) {  // Loop to take multiple samples
    sum += analogRead(tempSensorPin);      // Read ADC value from temperature sensor
    delay(readDelay);                      // Delay between each sample
  }
  int avgADC = sum / tempSamples;               // Calculate average ADC value
  float voltage = avgADC * (5.0 / 1023.0);      // Convert ADC value to voltage
  float temperature = (voltage - 0.5) * 100.0;  // Convert voltage to temperature
  return temperature;                           // Return calculated temperature
}
// Function to Calculate Fan Speed from Temperature and Sensitivity Level
int calculateFanSpeed(float temp, int sensitivity) {
  int pwmValue;                        // Variable for PWM value
  switch (sensitivity) {
    case 1:  // Low Sensitivity
      pwmValue = map(temp, 20, 40, 0, 255);   // Map temperature to PWM 
      break;
    case 2:  // Medium Sensitivity
      pwmValue = map(temp, 25, 35, 0, 255);   
      break;
    case 3:  // High Sensitivity
      pwmValue = map(temp, 30, 35, 0, 255);  
      break;
    default:
      pwmValue = 0;              
  }
  pwmValue = constrain(pwmValue, 0, 255);
  return pwmValue;                   
}
