#include <LiquidCrystal.h>

// LCD pins
int rs = 7;
int en = 8;
int d4 = 9;
int d5 = 10;
int d6 = 11;
int d7 = 12;

// Ultrasonic sensor pins
int trigPin = 3;
int echoPin = 2;

// Output component pins
int greenPin = 4;
int redPin = 5;
int buzzPin = 6;

int travelT;
float travelDist;
float targetDist;
float distInM;
float safeDist = 2.;

int delayT1 = 10;
int delayT2 = 25;
int delayT3 = 250;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(buzzPin, OUTPUT);
}

void loop() {
  // Sends and receives ultrasonic sound waves
  digitalWrite(trigPin, LOW);
  delayMicroseconds(delayT1);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(delayT1);
  digitalWrite(trigPin, LOW);
  travelT = pulseIn(echoPin, HIGH);
  delayMicroseconds(delayT2);

  // Calculates distance (in inches) from time using dimensional analysis
  travelDist = (travelT * 765. * 5280. * 12) / (3600. * 1000000);
  targetDist = travelDist / 2;
  // Converts inches to meters
  distInM = (targetDist * 2.54) / 100;

  // Displays distance in m
  lcd.setCursor(0,0);
  lcd.print("Distance: ");
  lcd.print(distInM);
  lcd.print("m");

  // If object is 2m or further,
  // displays 'safe' message, and green LED
  if (distInM >= safeDist) {
    lcd.setCursor(0,1);
    lcd.print("Safe. :)");
    digitalWrite(greenPin, HIGH);
    digitalWrite(redPin, LOW);
    digitalWrite(buzzPin, LOW);
  }
  // If object is within 2m,
  // displays 'unsafe' message, red LED, and sounds buzzer
  else {
    lcd.setCursor(0,1);
    lcd.print("Unsafe! >:(");
    digitalWrite(greenPin, LOW);
    digitalWrite(redPin, HIGH);
    digitalWrite(buzzPin, HIGH);
  }
  
  // Updates distance every quarter second
  delay(delayT3);
  lcd.clear(); 
}
