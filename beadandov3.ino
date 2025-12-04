#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo radarServo;

const int trigPin = 2;
const int echoPin = 3;
const int ledPin = 6;
const int buzzerPin = 7;
const int servoPin = 8;

const int potPin = A1;   // <-- Potméter középső lába

float ido, tav;

int fok = 0;
int lepes = 2;

void setup() {
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  radarServo.attach(servoPin);

  lcd.init();
  lcd.backlight();
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Radar indul...");
  delay(1000);
}

float measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  ido = pulseIn(echoPin, HIGH);
  tav = (ido * 0.0343) / 2;

  //Serial.print("Tavolsag: ");
  //Serial.println(tav);

  return tav;
}

void loop() {

  // --- POTMÉTER BEOLVASÁSA ---
  int potValue = analogRead(potPin);

  // A pásztázás sebessége (10–120 ms között)
  int sweepDelay = map(potValue, 0, 1023, 10, 120);

  radarServo.write(fok);

  tav = measureDistance();

  Serial.print("Szog: ");
  Serial.print(fok);
  Serial.print("  Tav: ");
  Serial.print(tav);
  Serial.print(" cm  Speed: ");
  Serial.println(sweepDelay);

  lcd.clear();

  // ********** KÉRT KIÍRÁS: SZÖG + SEBESSÉG EGY SORBAN **********
  lcd.setCursor(0, 0);
  lcd.print("Szog:");
  lcd.print(fok);
  lcd.print(" SPD:");
  lcd.print(sweepDelay);
  // **************************************************************

  lcd.setCursor(0, 1);

  if (tav <= 50 && tav > 0) {
    digitalWrite(ledPin, HIGH);
    digitalWrite(buzzerPin, HIGH);
    lcd.print("Behatolas!");
  } else {
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);
    lcd.print("Tav: ");
    lcd.print(tav, 1);
    lcd.print(" cm");
  }

  // Szervó irányváltás
  fok += lepes;
  if (fok >= 180 || fok <= 0) lepes = -lepes;

  // *** EZT SZABÁLYOZZA A POTMÉTER ***
  delay(sweepDelay);
}
