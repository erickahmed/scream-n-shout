/* Arduino libraries */
#include <Arduino.h>
#include <EEPROM.h>

/* definitions */
#define uint8_t const unsigned char

uint8_t buzzer = 6;
uint8_t led = 7;
uint8_t echo = 8;
uint8_t trig = 9;

bool breach;

bool startupBreach(bool breach) {
  if (breach == true) {
    EEPROM.write(0, true);
    return true;
    }
  else {
    EEPROM.write(0, false);
    return false;
    }
}

void scream(bool x) {
  if (x) {
    for(int i = 0; i<100; i++){
    tone(buzzer, 4900);
    delay(1000);
    digitalWrite(led, HIGH);
    noTone(buzzer);
    delay(1000);
    digitalWrite(led, LOW);
    }
  }

  else digitalWrite(buzzer, LOW);
}

void blip(bool x) {
  if (x) digitalWrite(led, HIGH);
}

void silentAlarm() {
    while (true) {
    blip(true);
  }
}

void setAlarm() {
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

  return distance;
  delay(100);
}

void setup() {
  pinMode(button, INPUT);
  pinMode(echo, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(trig, OUTPUT);

  tone(buzzer, 4900);
  digitalWrite(led, HIGH);
  delay(1000);
  noTone(buzzer);
  digitalWrite(led, LOW);

  delay(15000);

  if(EEPROM.read(0) == true) {
    silentAlarm();
  }
}

void loop() {

  long ultra = measureDistance();
  long dist = 20;

  if (ultra <= dist) {
    breach = true;
    startupBreach(breach);
    setAlarm();
  }
}
