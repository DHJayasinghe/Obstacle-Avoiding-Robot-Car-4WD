/*SHARP GP2Y0A21YK0F IR distance sensor with Arduino and SharpIR library example code. More info: https://www.makerguides.com */

// Include the library:
#include <SharpIR.h>

// Define model and input pin:
#define IRPin A7
#define model 1080

// Create variable to store the distance:
unsigned int distance_cm;

/* Model :
  GP2Y0A02YK0F --> 20150
  GP2Y0A21YK0F --> 1080
  GP2Y0A710K0F --> 100500
  GP2YA41SK0F --> 430
*/

// Create a new instance of the SharpIR class:
SharpIR mySensor = SharpIR(IRPin, model);

void setup() {
  // Begin serial communication at a baudrate of 9600:
  Serial.begin(9600);
}

void loop() {
  // Get a distance measurement and store it as distance_cm:
  distance_cm = mySensor.distance();

  // Print the measured distance to the serial monitor:
  //Serial.print("Mean distance: ");
  Serial.print("Library Calculation: ");
  Serial.print(String(distance_cm) + " cm");
  Serial.print(" / ");
  Serial.print("Manual Calculation: ");
  //Formulae = 4800/(analog reading - 20)
  int reading = analogRead(IRPin);
  float result = (float)(4800 / (reading - 20));
  Serial.print(String(result) + " cm");
  Serial.println();

  delay(1000);
}
