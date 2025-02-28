/****************************************************
* MC_Lab4_Q1.ino                                    *
* Sebastian Posada,                                 *
* 2/28/2025 8AM,                                    *
*                                                   *
*Used Chat GPT to help with debugging and code Flow *
****************************************************/

#define BITSIZE 12                                                // Define the number of bits
const float VREF = 3.6;                                           // Reference voltage 
float testVoltages[] = {0.42, 0.83, 1.65, 2.752};                 // Array of test voltages
int numVoltages = sizeof(testVoltages) / sizeof(testVoltages[0]); // Calculate the number of test voltages
int currentTest = 0;                                              // Cycle through each test voltage

/********************************
**************SETUP**************
********************************/

void setup() {
  Serial.begin(9600);  // Begin serial communication 
  delay(1000);  
}

/********************************
************MAIN-LOOP************
********************************/

// Cycle through each test voltage
void loop() {
  if (currentTest < numVoltages) {
    // Call the SAR ADC function for the current test voltage
    SAR_ADC(testVoltages[currentTest]);  
    // Move to the next test voltage
    currentTest++;
    delay(2000);  
  }
}

/********************************
**********SAR-FUNCTION***********
********************************/
void SAR_ADC(float Vin) {
    float thresh = VREF / 2;         // Threshold set to half of reference 
    float quantized = 0;             // Store the quantized output
    int bits[BITSIZE] = {0};         // Array for binary output of the SAR ADC
    int digitalOutput = 0;           // Store digital output in decimal form
    Serial.print("Input Voltage: "); 
    Serial.print(Vin, 4);            
    Serial.println("V");
    // Loop through each bit to determine its value
    for (int count = 0; count < BITSIZE; count++) {
        thresh = VREF / pow(2, count + 1);              // Update the threshold by halving it in each iteration
        quantized += thresh;                            // Add the current threshold to the quantized value 
        if (Vin >= quantized) {                         // Compare the input voltage with the quantized value
            bits[count] = 1;                            // If input voltage is greater than or equal to quantized value Set the current bit to 1
        } else {
            bits[count] = 0;                
            quantized -= thresh;            
        }
    }
    // Calculate Digital Output in Decimal Form
    for (int i = 0; i < BITSIZE; i++) {
        digitalOutput += bits[i] * (1 << (BITSIZE - 1 - i));
    }
    // Calculate Reconstructed Analog Voltage
    float reconstructedVoltage = (digitalOutput * VREF) / (pow(2, BITSIZE) - 1);
    // Display the digital output in binary form on the serial monitor
    Serial.print("Digital Output (");
    Serial.print(BITSIZE);
    Serial.print("-bit): ");
    // Loop through each bit and print it
    for (int i = 0; i < BITSIZE; i++) {
        Serial.print(bits[i]);
    }
    // Display the digital output in decimal form
    Serial.print(" Decimal: ");
    Serial.println(digitalOutput);
    Serial.print("Reconstructed Voltage: ");    // Display the reconstructed analog voltage
    Serial.print(reconstructedVoltage, 4);      // Print with 4 decimal places
    Serial.println("V");
}
