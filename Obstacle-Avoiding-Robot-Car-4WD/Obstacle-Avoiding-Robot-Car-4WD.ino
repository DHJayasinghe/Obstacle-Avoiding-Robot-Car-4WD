#include <AFMotor.h> //add Adafruit Motor Shield library
#include <Servo.h>        //add Servo Motor library            
#include <NewPing.h>      //add Ultrasonic sensor library

#define TRIG_PIN 14 // Digital Pin # 14 on the Motor Drive Shield for the the ultrasonic sensor
#define ECHO_PIN 15 // Digital Pin # 15 on the Motor Drive Shield for the ultrasonic sensor
#define SERVO1_PIN 10 // Digital Pin # 10 on the Motor Drive Shield for the Servo motor

#define MAX_DISTANCE 300 // sets maximum useable sensor measuring distance to 300cm
#define MAX_SPEED 90 // sets speed of DC traction motors to 150/250 or about 70% of full speed - to get power drain down.
#define MAX_SPEED_OFFSET 30 // 40 this sets offset to allow for differences between the two DC traction motors
#define COLL_DIST 30 // sets distance at which robot stops and reverses to 30cm (collision distance)
#define TURN_DIST COLL_DIST+20 // sets distance at which robot veers away from object

#define FRONT_RIGHT_BARRIER_PIN 16 // right obstacle detection sensor (Infrared barrier sensor)
#define FRONT_LEFT_BARRIER_PIN 17 // left obstacle detection sensor (Infrared barrier sensor)

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); // sets up sensor library to use the correct pins to measure distance.

AF_DCMotor leftMotor1(1, MOTOR12_1KHZ); // create motor #1 using M1 output on Motor Drive Shield, set to 1kHz PWM frequency
AF_DCMotor leftMotor2(4, MOTOR12_1KHZ); // create motor #4, using M2 output, set to 1kHz PWM frequency
AF_DCMotor rightMotor1(2, MOTOR34_1KHZ);// create motor #2, using M3 output, set to 1kHz PWM frequency
AF_DCMotor rightMotor2(3, MOTOR34_1KHZ);// create motor #3, using M4 output, set to 1kHz PWM frequency
Servo myservo;  // create servo object to control a servo

int leftDistance, rightDistance; //distances on either side
int curDist = 0;
String motorSet = "";
int speedSet = 0;

//-------------------------------------------- SETUP LOOP ----------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  myservo.attach(SERVO1_PIN);  // attaches the servo pin for SERVO_1 on the Motor Drive Shield to the servo object
  myservo.write(90); // tells the servo to position at 90-degrees ie. facing forward.
  pinMode(FRONT_RIGHT_BARRIER_PIN, INPUT);
  pinMode(FRONT_LEFT_BARRIER_PIN, INPUT);
  delay(1000); // delay for one seconds
}
//------------------------------------------------------------------------------------------------------------------------------------

//---------------------------------------------MAIN LOOP ------------------------------------------------------------------------------
void loop() {
  myservo.write(90);  // move eyes forward
  delay(90);
  bool isFrObstacled = digitalRead(FRONT_RIGHT_BARRIER_PIN); // if front right has obstacle
  bool isFlObstacled = digitalRead(FRONT_LEFT_BARRIER_PIN); // if front left has obstacle
  curDist = readPing();   // read distance

  if (curDist < COLL_DIST || isFrObstacled==LOW || isFlObstacled==LOW) {
    changePath(); // if forward is blocked change direction
  }
  moveForward();  // move forward
  delay(500);
}
//-------------------------------------------------------------------------------------------------------------------------------------

void changePath() {
  moveStop();   // stop forward movement
  myservo.write(36);  // check distance to the right
  delay(500);
  rightDistance = readPing(); //set right distance
  delay(500);
  myservo.write(144);  // check distace to the left
  delay(700);
  leftDistance = readPing(); //set left distance
  delay(500);
  myservo.write(90); //return to center
  delay(100);

  compareDistance(leftDistance, rightDistance);
}


void compareDistance(int leftDist, int rightDist)  // find the longest distance
{
  if (leftDist > rightDist) //if left is less obstructed
  {
    turnLeft();
  }
  else if (rightDist > leftDist) //if right is less obstructed
  {
    turnRight();
  }
  else //if they are equally obstructed
  {
    turnAround();
  }
}


//-------------------------------------------------------------------------------------------------------------------------------------
int readPing() { // read the ultrasonic sensor distance
  delay(70);
  unsigned int uS = sonar.ping_median();
  int cm = uS / US_ROUNDTRIP_CM;
  // Serial.println(cm);
  return cm;
}
//-------------------------------------------------------------------------------------------------------------------------------------
void moveStop() {
  leftMotor1.run(RELEASE);  // stop the motors.
  leftMotor2.run(RELEASE);
  rightMotor1.run(RELEASE);
  rightMotor2.run(RELEASE);
}
//-------------------------------------------------------------------------------------------------------------------------------------
void moveForward() {
  motorSet = "FORWARD";
  leftMotor1.run(FORWARD);      // turn it on going forward
  leftMotor2.run(FORWARD);      // turn it on going forward
  rightMotor1.run(FORWARD);     // turn it on going forward
  rightMotor2.run(FORWARD);     // turn it on going forward
  for (speedSet = 0; speedSet < MAX_SPEED; speedSet += 2) // slowly bring the speed up to avoid loading down the batteries too quickly
  {
    leftMotor1.setSpeed(speedSet);
    leftMotor2.setSpeed(speedSet);
    rightMotor1.setSpeed(speedSet);
    rightMotor2.setSpeed(speedSet);
    delay(5);
  }
}
//-------------------------------------------------------------------------------------------------------------------------------------
void moveBackward() {
  motorSet = "BACKWARD";
  leftMotor1.run(BACKWARD);     // turn it on going backward
  leftMotor2.run(BACKWARD);     // turn it on going backward
  rightMotor1.run(BACKWARD);    // turn it on going backward
  rightMotor2.run(BACKWARD);    // turn it on going backward
  for (speedSet = 0; speedSet < MAX_SPEED; speedSet += 2) // slowly bring the speed up to avoid loading down the batteries too quickly
  {
    leftMotor1.setSpeed(speedSet);
    leftMotor2.setSpeed(speedSet);
    rightMotor1.setSpeed(speedSet);
    rightMotor2.setSpeed(speedSet);
    delay(5);
  }
}
//-------------------------------------------------------------------------------------------------------------------------------------
void turnRight() {
  motorSet = "RIGHT";
  leftMotor1.run(FORWARD);      // turn motor 1 forward
  leftMotor2.run(FORWARD);      // turn motor 2 forward
  rightMotor1.run(BACKWARD);    // turn motor 3 backward
  rightMotor2.run(BACKWARD);    // turn motor 4 backward
  rightMotor1.setSpeed(speedSet + MAX_SPEED_OFFSET);
  rightMotor2.setSpeed(speedSet + MAX_SPEED_OFFSET);
  delay(1500); // run motors this way for 1500
  motorSet = "FORWARD";
  leftMotor1.run(FORWARD);      // set both motors back to forward
  leftMotor2.run(FORWARD);
  rightMotor1.run(FORWARD);
  rightMotor2.run(FORWARD);
}
//-------------------------------------------------------------------------------------------------------------------------------------
void turnLeft() {
  motorSet = "LEFT";
  leftMotor1.run(BACKWARD);      // turn motor 1 backward
  leftMotor2.run(BACKWARD);      // turn motor 2 backward
  leftMotor1.setSpeed(speedSet + MAX_SPEED_OFFSET);
  leftMotor2.setSpeed(speedSet + MAX_SPEED_OFFSET);
  rightMotor1.run(FORWARD);     // turn motor 3 forward
  rightMotor2.run(FORWARD);     // turn motor 4 forward
  delay(1500); // run motors this way for 1500
  motorSet = "FORWARD";
  leftMotor1.run(FORWARD);      // turn it on going forward
  leftMotor2.run(FORWARD);      // turn it on going forward
  rightMotor1.run(FORWARD);     // turn it on going forward
  rightMotor2.run(FORWARD);     // turn it on going forward
}
//-------------------------------------------------------------------------------------------------------------------------------------
void turnAround() {
  motorSet = "RIGHT";
  leftMotor1.run(FORWARD);      // turn motor 1 forward
  leftMotor2.run(FORWARD);      // turn motor 2 forward
  rightMotor1.run(BACKWARD);    // turn motor 3 backward
  rightMotor2.run(BACKWARD);    // turn motor 4 backward
  rightMotor1.setSpeed(speedSet + MAX_SPEED_OFFSET);
  rightMotor2.setSpeed(speedSet + MAX_SPEED_OFFSET);
  delay(1700); // run motors this way for 1700
  motorSet = "FORWARD";
  leftMotor1.run(FORWARD);      // set both motors back to forward
  leftMotor2.run(FORWARD);
  rightMotor1.run(FORWARD);
  rightMotor2.run(FORWARD);
}
