#define leftMostPin 2
#define leftPin 3
#define middlePin 4
#define rightPin 5
#define rightMostPin 6

void setup()
{
  Serial.begin(9600);
  pinMode(leftMostPin, INPUT);
  pinMode(leftPin, INPUT);
  pinMode(middlePin, INPUT);
  pinMode(rightPin, INPUT);
  pinMode(rightMostPin, INPUT);

}

void loop()
{
  boolean leftMostVal = digitalRead(leftMostPin); // read the value of tcrt5000
  boolean leftVal = digitalRead(leftPin);
  boolean middleVal = digitalRead(middlePin);
  boolean rightVal = digitalRead(rightPin);
  boolean rightMostVal = digitalRead(rightMostPin);
  Serial.print("LM: ");
  Serial.print(leftMostVal);
  Serial.print("\tL: ");
  Serial.print(leftVal);
  Serial.print("\tM: ");
  Serial.print(middleVal);
  Serial.print("\tR: ");
  Serial.print(rightVal);
  Serial.print("\tRM: ");
  Serial.print(rightMostVal);
  Serial.println("\n");
}
