#define pirSensor 3

void setup() {
  Serial.begin(9600);
  pinMode(pirSensor,INPUT);
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  bool sensorValue = digitalRead(pirSensor);
  if (sensorValue == HIGH) {
    Serial.println("Detection");
  }
  else {
    Serial.println("No detection");
  }
  delay(300);
}
