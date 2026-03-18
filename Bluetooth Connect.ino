#include "Arduino.h"
#include <SoftwareSerial.h>
const int ledPin = 2;
const int ENA = 3;
const int ENB = 5;
const int IN1 = 6;
const int IN2 = 7;
const int IN3 = 8;
const int IN4 = 11;
// const int trigPin = 3;
// const int echoPin = 5;

const int rxPin = 10;
const int txPin = 9;

SoftwareSerial BTSerial(rxPin, txPin);
void moveFunction(String message);
void moveForward();
void moveLeft();
void moveRight();
void moveBackward();
void stop();
int getNextNumber(String message, int textCursor);
String parseMessage();

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  analogWrite(ENA, 162);
  analogWrite(ENB, 200);
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  BTSerial.begin(9600);
  Serial.begin(9600);
}

const char START_TOKEN = '$';
const char END_TOKEN = ';';
const char DELIMIT_TOKEN = '&';
const int CHAR_TIMEOUT = 20;

bool waitingForStartToken = true;
String messageBuffer = "";

void loop() {

  if (BTSerial.available()){
    String direction = BTSerial.readString();
    direction.trim();
    Serial.println(direction); 
    if (direction == "f"|| direction == "f;"){
      moveForward();
    }
    else if (direction == "b"|| direction == "b;"){
      moveBackward();
    }
    else if (direction == "l"|| direction == "l;"){
      moveLeft();
    }
    else if (direction == "r"|| direction == "r;"){
      moveRight();
    }
    else{
      stop();
    }
  }
}


void moveForward(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveLeft(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void moveRight(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveBackward(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stop(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

String parseMessage(){
  // Process one character per loop iteration
  if (Serial.available()) {
    char nextData;

    if (waitingForStartToken) {
      do {
        nextData = Serial.read();
      } while ((nextData != START_TOKEN) && Serial.available());
      // Ignore everything until we see '$'
      if (nextData == START_TOKEN) {
        Serial.println("Message start...");
        messageBuffer = "";
        waitingForStartToken = false;
      }
    } 
    if (!waitingForStartToken && Serial.available()) {
      // We're inside a message, accumulate characters
      do {
        nextData = Serial.read();
        Serial.println(nextData);
        messageBuffer += nextData;
      } while ((nextData != END_TOKEN) && Serial.available());

      if (nextData == END_TOKEN) {
        messageBuffer = messageBuffer.substring(0, messageBuffer.length() - 1);
        Serial.println("Message complete: " + messageBuffer);
        moveFunction(messageBuffer);
        messageBuffer = "";
        waitingForStartToken = true;
      } 

      // Timeout: too many characters without END_TOKEN
      if (messageBuffer.length() > CHAR_TIMEOUT) {
        Serial.println("Message Timeout: " + messageBuffer);
        messageBuffer = "";
        waitingForStartToken = true;
      }
    }
  }
}
