/* Arduino libraries */
#include <Arduino.h>
#include <EEPROM.h>

/* definitions */
#define uint8_t const unsigned char
#define iter 25;
//#define dist 6; //FIXME: measure actual distance

uint8_t button = 5;
uint8_t buzzer = 6;
uint8_t led = 7;
uint8_t echo = 8;
uint8_t trig = 9;

bool breach;

bool startupBreach() {
  if (EEPROM.read(breach) == true) return true;
  else return false;
}

void scream(bool x) {
  if (x) {
    digitalWrite(buzzer, HIGH);
    delayMicroseconds(1000);
    digitalWrite(buzzer, LOW);
    delay(1000);
  }
  else digitalWrite(buzzer, LOW);
}

void blip(bool x) {
  if (x && ((digitalRead(led)) == LOW)) digitalWrite(led, HIGH);
  else{
    if(x == false) digitalWrite(led, LOW);
    //elseif won't compile lmao
   }
}

void silentAlarm() {
    while (true) {
    blip(true);
  }
}

void setAlarm() {
  EEPROM.write(breach, true);

  scream(true);
  silentAlarm();
}

long ms2cm(long ms) {
  return (ms / 29 / 2);
}

long measureDistance() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long duration = pulseIn(echo, HIGH);
  long distance = ms2cm(duration);

  Serial.print(distance);
  Serial.print(" cm");
  Serial.println();

  return distance;
  delay(100);
}

void setup() {
  Serial.begin(9600);
  blip(false);
  scream(false);

  pinMode(button, INPUT);
  pinMode(echo, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(trig, OUTPUT);

  if (startupBreach() == true) silentAlarm();
}

void loop() {
  if (digitalRead(button) == true) delayMicroseconds(15000);

  long ultra = measureDistance();
  long dist = 20;

  if (ultra <= dist) {
    breach = true;
  }
  else {
    breach = false;
  }

  if (breach == true) {
    setAlarm();
  }
}
