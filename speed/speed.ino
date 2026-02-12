#include <Servo.h>
int ledPin = 2;
int ENA = 3;
int ENB = 11;
int IN1 = 6;
int IN2 = 7;
int IN3 = 8;
int IN4 = 9;
int trigPin = 3;
int echoPin = 5;
// int distance = 20;
// int distance_collision = 40;
Servo myServo;
void setup() {

  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(2,INPUT);

  // analogWrite(ENA, 200);
  // analogWrite(ENB, 150);


  // myServo.write(90);
  // myServo.attach(3);
}
void moveForward(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void measureAndPrintDistance(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  long distance = (duration / 2) * 0.0343;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
}

void loop() {

  measureAndPrintDistance();
  delay(200);
  if (distance > distance_collision){
    moveForward();
    delay(500);
  } else{
    // Stop();
    myServo.write(0);
    delay(200);
    measureAndPrintDistance();
    delay(200);
  }
  measureAndPrintDistance();
  delay(200);

  moveForward();
  // delay(2000);

  // myServo.write(0);
  // delay(1000);
  // myServo.write(90);
  // delay(1000);
  // myServo.write(180);
  // delay(1000);
}
