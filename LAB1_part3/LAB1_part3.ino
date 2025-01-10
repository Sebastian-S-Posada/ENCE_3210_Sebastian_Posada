#define ARRAY_SIZE 50
#define INT_MIN (-2147483648) // Define INT_MIN

int myArray[ARRAY_SIZE]; // Array of integers
int* memoryLocation = &myArray[ARRAY_SIZE]; // Pointer to memory after array

void setup() {
  Serial.begin(9600);

  // Initialize array with random values
  for (int i = 0; i < ARRAY_SIZE; i++) {
    myArray[i] = random(1, 101);
  }

  int max1 = INT_MIN, max2 = INT_MIN;

  // Find the first and second maximum
  for (int i = 0; i < ARRAY_SIZE; i++) {
    if (myArray[i] > max1) {
      max2 = max1;
      max1 = myArray[i];
    } else if (myArray[i] > max2 && myArray[i] != max1) {
      max2 = myArray[i];
    }
  }

  // Write second maximum to memory after the array
  *memoryLocation = max2;

  // Debugging output
  Serial.println("Array Values:");
  for (int i = 0; i < ARRAY_SIZE; i++) {
    Serial.print(myArray[i]);
    Serial.print(" ");
  }
  Serial.println();

  Serial.print("Second Maximum: ");
  Serial.println(max2);
  Serial.print("Value at Memory Location After Array: ");
  Serial.println(*memoryLocation);
}

void loop() {
  // Empty loop
}
