/*
 * File: servo_radar_demo.ino
 * www.bluetin.io
 * 
 * Guide: https://www.bluetin.io/ultrasonic-sensor-sweeper-servo-arduino-code/
 * 
 * MIT License
 * https://github.com/MarkAHeywood/bluetin/blob/master/ultrasonic-sensor-sweeper-servo-arduino-code/servo_radar_demo.ino
 */
 
#include <Servo.h>
 
Servo panServo;
 
//---------- Ultrasonic Sensor Pin Configuration -------------
const uint8_t ECHO_PIN = 15;
const uint8_t TRIGGER_PIN = 14;
 
//---------- Servo Pin Configuration -------------
const uint8_t panServoPin = 10;
 
//---------- Adjust the following to fit servo limit. ---------- 
const int panLimitMin = 550;
const int panLimitMax = 2400;
const int panServoCentre = 1450;
 
//---------- Other Constant ----------
const long servoDelayTime = 50000; // Delay (micros) between servo moves (rotation speed).
const int stepSize = 20;
const int sensorTimeout = 17493;  // Microseconds
 
//---------- Values we want to keep track of throughout the program. ----------
long servoTimestamp = 0;
bool servoDirectionClockwise = true;
int panPosition = 0; // Microseconds, with initial servo pan position.
 
//========== Functions ===========
 
uint16_t echo(bool isCM) {
  long echoDuration;
  long distance;
 
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(5);
  digitalWrite(TRIGGER_PIN, LOW);
 
  echoDuration = pulseIn(ECHO_PIN, HIGH, sensorTimeout); // Returns microseconds
  //echoDuration = pulseIn(ECHO_PIN, HIGH); // Returns microseconds
 
  if (isCM) {
    // Distance in cm conversion.
    distance = echoDuration / 29 / 2; // 29.15 microseconds per cm
    /*
    Serial.print(distance);
    Serial.println("cm");
    */
  } else {
    // Distance in inches conversion.
    distance = echoDuration / 74 / 2;
    /*
    Serial.print(distance);
    Serial.println("in");
    */
  }
  //delay(1);
  return distance;
}
 
 
bool servoControl() {
  bool panPositionUpdate = false;
  if (micros() > (servoTimestamp + servoDelayTime)) {
    if (servoDirectionClockwise == true) {
      panPosition = panPosition - stepSize;
      if (panPosition < panLimitMin) {
        servoDirectionClockwise = false;
      } else {
        panPositionUpdate = true; // Update servo position
      }
    }
 
    if (servoDirectionClockwise == false) {
      panPosition = panPosition + stepSize;
      if (panPosition > panLimitMax) {
        servoDirectionClockwise = true;
      } else {
        panPositionUpdate = true; // Update servo position
      }
    }
 
    if (panPositionUpdate == true) {
      panServo.writeMicroseconds(panPosition);
      servoTimestamp = micros();
      //Serial.println(panPosition);
    }
  }
  return panPositionUpdate;
}
 
//========== Main Functions ==========
 
void setup() {
  Serial.begin(115200);
  // Configure pins.
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  panServo.attach(panServoPin);
  panPosition = panServoCentre;
  panServo.writeMicroseconds(panPosition);
  servoTimestamp = micros();
}
 
 
void loop() {
  if (servoControl() == true) { // Has the servo position updated?
    // Get distance update.
    uint16_t distanceCM = echo(true); // Report in cm = true.
    Serial.print("Rotation Clockwise: ");
    Serial.print(servoDirectionClockwise);
    Serial.print(", Servo Position: ");
    Serial.print(panPosition);
    Serial.print(", Distance: ");
    Serial.print(distanceCM);
    Serial.println("cm");
  }
}
