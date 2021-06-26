#include <LiquidCrystal.h>

// LCD pins
const int rs = 7;
const int en = 8;
const int d4 = 9;
const int d5 = 10;
const int d6 = 11;
const int d7 = 12;

// Ultrasonic sensor pins
const int trigPin = 3;
const int echoPin = 2;

// Output component pins
const int greenPin = 4;
const int redPin = 5;
const int buzzPin = 6;

int travelT;
float travelDist;
float targetDist;
float distInM;
const float safeDist = 2.;

const int delayT1 = 10;
const int delayT2 = 25;
const int delayT3 = 250;
const int buzzerDelay1 = 100;
const int buzzerDelay2 = 500;

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
  
  travelT = sendWave();

  distInM = calculateDistance();

  displayDistance();

  warnUser();
  
  updateReading();
}

// Sends and receives ultrasonic sound waves
int sendWave() {
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(delayT1);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(delayT1);
  digitalWrite(trigPin, LOW);
  travelT = pulseIn(echoPin, HIGH);
  delayMicroseconds(delayT2);

  return travelT;
}

// Calculates distance (in inches) from time using dimensional analysis and speed of sound
float calculateDistance() {
    
  travelDist = (travelT * 767. * 5280. * 12) / (3600. * 1000000);
  targetDist = travelDist / 2;
  // Converts inches to meters
  distInM = (targetDist * 2.54) / 100;
  return distInM;
}

// Displays distance in m
void displayDistance() {
   
  lcd.setCursor(0,0);
  lcd.print("Distance: ");
  lcd.print(distInM);
  lcd.print("m");
}

// Warn user whether their distance from object is safe or unsafe
void warnUser() {
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
    delay(buzzerDelay1);
    digitalWrite(buzzPin, LOW);
    delay(buzzerDelay2);
  }
}

// Updates distance every quarter second, and resets display
void updateReading() {
  delay(delayT3);
  lcd.clear(); 
}
