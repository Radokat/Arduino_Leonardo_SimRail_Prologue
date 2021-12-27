/*
  POC for Simrail EN96 Leonardo to keyboard.
  A0 for power handle.
  A1 for local brake.
*/
#define FASTADC 1

// defines for setting and clearing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#include "Keyboard.h"

void setup() {
#if FASTADC
  // set prescale to 16
  sbi(ADCSRA, ADPS2) ;
  cbi(ADCSRA, ADPS1) ;
  cbi(ADCSRA, ADPS0) ;
#endif

  Serial.begin(115200);
  Keyboard.begin();
}

void loop() {
  unsigned long current_time = 0;
  const int bdoorleft = 12;
  const int baws = 11;
  const int bdoorright = 10;
  int bdoorleftState = 0;
  int bdoorlefttoggle = 0;
  int bdoorrighttoggle = 0;
  int bdoorawsState = 0;
  int bdoorrightState = 0;
  int lastpowerstate = 20; // Default position should be hold brake state - middle of the fader.
  int lastbrakestate = 20; // Default position should be hold brake state - middle of the fader.
  pinMode(2, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  pinMode(bdoorleft, INPUT);
  pinMode(baws, INPUT);
  pinMode(bdoorright, INPUT);
  static int brakedelay = 200;   // Delay value for brake side of lever.
  static int powerdelay = 200;   // Delay value for power side of lever.
  static int stickydelay = 200;  // Delay value for sticky operations like EB and switch between brake/power.
  static int buttondelay = 120;
  //Keypad
  const byte KEYPAD_0 = 234;
  const byte KEYPAD_1 = 225;
  const byte KEYPAD_2 = 226;
  const byte KEYPAD_3 = 227;
  const byte KEYPAD_4 = 228;
  const byte KEYPAD_5 = 229;
  const byte KEYPAD_6 = 230;
  const byte KEYPAD_7 = 231;
  const byte KEYPAD_8 = 232;
  const byte KEYPAD_9 = 233;
  const byte KEYPAD_ASTERIX = 221;
  const byte KEYPAD_ENTER = 224;
  const byte KEYPAD_MINUS = 222;
  const byte KEYPAD_PERIOD = 235;
  const byte KEYPAD_PLUS = 223;
  const byte KEYPAD_SLASH = 220;
  while (digitalRead(2) == HIGH) { //In case of hung inputs ground pin 2 and restart the Leonardo.
    bdoorleftState = digitalRead(bdoorleft);
    bdoorawsState = digitalRead(baws);
    bdoorrightState = digitalRead(bdoorright);
    int sensorValuePower = analogRead(A0);
    int sensorValueBrake = analogRead(A1);
    // Buttons
    if (bdoorleftState == HIGH && bdoorlefttoggle == 0 ) {
      current_time = millis();
      while (millis() < (current_time + buttondelay)) {
        Keyboard.press('i');
        bdoorleftState = LOW;
      }
      bdoorlefttoggle = 10;
      Keyboard.release('i');
      while (millis() < (current_time + buttondelay * 4)) {
      }
    }
    if (bdoorleftState == HIGH && bdoorlefttoggle == 10 ) {
      current_time = millis();
      while (millis() < (current_time + buttondelay)) {
        Keyboard.press('p');
        bdoorleftState = LOW;
      }
      bdoorlefttoggle = 0;
      Keyboard.release('p');
      while (millis() < (current_time + buttondelay * 4)) {
      }
    }
    if (bdoorawsState == HIGH) {
      current_time = millis();
      while (millis() < (current_time + buttondelay)) {
        Keyboard.press(0x20);
        bdoorrightState = LOW;
      }
      Keyboard.release(0x20);
      while (millis() < (current_time + buttondelay * 4)) {
      }
    }
    if (bdoorrightState == HIGH && bdoorrighttoggle == 0 ) {
      current_time = millis();
      while (millis() < (current_time + buttondelay)) {
        Keyboard.press('p');
        bdoorrightState = LOW;
      }
      bdoorrighttoggle = 10;
      Keyboard.release('p');
      while (millis() < (current_time + buttondelay * 4)) {
      }
    }
    if (bdoorrightState == HIGH && bdoorrighttoggle == 10 ) {
      current_time = millis();
      while (millis() < (current_time + buttondelay)) {
        Keyboard.press('i');
        bdoorrightState = LOW;
      }
      bdoorrighttoggle = 0;
      Keyboard.release('i');
      while (millis() < (current_time + buttondelay * 4)) {
      }
    }
    // Power handle.
    if (sensorValuePower >= 0 && sensorValuePower <= 20 && lastpowerstate != 0) {
      if (lastpowerstate == 10)
      {
        Serial.println("Increase power");
        if (sensorValuePower >= 0 && sensorValuePower <= 20) {
          digitalWrite(13, HIGH);
          Keyboard.press(KEYPAD_PLUS);
          sensorValuePower = analogRead(A0);
        }
        lastpowerstate = 0;
        sensorValuePower = analogRead(A0);
      }
    }
    if (sensorValuePower >= 100 && sensorValuePower <= 200 && lastpowerstate != 10) {
      if (lastpowerstate == 0)
      {
        Keyboard.release(KEYPAD_PLUS);
        Keyboard.release(KEYPAD_MINUS);
        digitalWrite(13, LOW);
        Serial.println("Maintain power");
        lastpowerstate = 10;
        sensorValuePower = analogRead(A0);
      }
      if (lastpowerstate == 20)
      {
        Keyboard.write(KEYPAD_PLUS);
        Serial.println("Maintain power");
        lastpowerstate = 10;
        sensorValuePower = analogRead(A0);
      }
    }
    if (sensorValuePower >= 410 && sensorValuePower <= 600 && lastpowerstate != 20) {
      if (lastpowerstate == 10)
      {
        Keyboard.release(KEYPAD_PLUS);
        Keyboard.release(KEYPAD_MINUS);
        Keyboard.write(KEYPAD_MINUS);
        Serial.println("Maintain brake");
        lastpowerstate = 20;
        sensorValuePower = analogRead(A0);
      }
      if (lastpowerstate == 30)
      {
        Keyboard.release(KEYPAD_PLUS);
        Keyboard.release(KEYPAD_MINUS);
        digitalWrite(13, LOW);
        Serial.println("Maintain brake");
        lastpowerstate = 20;
        sensorValuePower = analogRead(A0);
      }
    }
    if (sensorValuePower >= 610 && sensorValuePower <= 800 && lastpowerstate != 30) {
      if (lastpowerstate == 20)
      {
        Keyboard.press(KEYPAD_MINUS);
        digitalWrite(13, HIGH);
        Serial.println("Increase brake");
        lastpowerstate = 30;
        sensorValuePower = analogRead(A0);
      }
      if (lastpowerstate == 40)
      {
        Keyboard.release(KEYPAD_PLUS);
        Keyboard.release(KEYPAD_MINUS);
        Keyboard.press(KEYPAD_PLUS);
        digitalWrite(13, HIGH);
        Serial.println("Increase brake");
        lastpowerstate = 30;
        sensorValuePower = analogRead(A0);
      }
    }
    if (sensorValuePower >= 810 && sensorValuePower <= 900 && lastpowerstate != 40) {
      if (lastpowerstate == 30)
      {
        Keyboard.release(KEYPAD_PLUS);
        Keyboard.release(KEYPAD_MINUS);
        digitalWrite(13, LOW);
        Keyboard.write(KEYPAD_MINUS);
        Serial.println("Full Brake");
        lastpowerstate = 40;
        sensorValuePower = analogRead(A0);
      }
      if (lastpowerstate == 50) {
        Keyboard.release(KEYPAD_PLUS);
        Keyboard.release(KEYPAD_MINUS);
        digitalWrite(13, LOW);
        lastpowerstate = 40;
      }
    }
    if (sensorValuePower >= 1010 && sensorValuePower <= 1023 && lastpowerstate != 50) {
      if (lastpowerstate < 50 ) {
        if (sensorValuePower >= 1010 && sensorValuePower <= 1023) {
          Keyboard.release(KEYPAD_PLUS);
          Keyboard.release(KEYPAD_MINUS);
          digitalWrite(13, HIGH);
          Keyboard.press(KEYPAD_MINUS);
          Serial.println("Emergency brake");
          sensorValuePower = analogRead(A0);
        }
        lastpowerstate = 50;

      }
    }
    /// Local brake handle.
    if (sensorValueBrake >= 0 && sensorValueBrake <= 100 && lastbrakestate != 0) {
      if (lastbrakestate == 10)
      {
        if (sensorValueBrake >= 0 && sensorValueBrake <= 100) {
          Keyboard.release(KEYPAD_3);
          Keyboard.release(KEYPAD_9);
          Keyboard.press(KEYPAD_9);
          Serial.println("Release brake");
          Serial.println("up");
          Keyboard.press(KEYPAD_9);
          sensorValueBrake = analogRead(A1);
        }
        lastbrakestate = 0;
        sensorValueBrake = analogRead(A1);
        Keyboard.release(KEYPAD_3);
        Keyboard.release(KEYPAD_9);
      }
    }
    if (sensorValueBrake >= 110 && sensorValueBrake <= 200 && lastbrakestate != 10) {
      if (lastbrakestate == 0)
        Keyboard.press(KEYPAD_3);
      {
        if (sensorValueBrake >= 110 && sensorValueBrake <= 200) {
          digitalWrite(13, HIGH);
          Serial.println("Decrease brake");
          Keyboard.press(KEYPAD_9);
          sensorValueBrake = analogRead(A1);
        }
        lastbrakestate = 10;
        digitalWrite(13, LOW);
        sensorValueBrake = analogRead(A1);
      }
      if (lastbrakestate == 20)
      {
        if (sensorValueBrake >= 210 && sensorValueBrake <= 400) {
          digitalWrite(13, HIGH);
          Serial.println("Decrease brake");
          Keyboard.press(KEYPAD_9);
          sensorValueBrake = analogRead(A1);
        }
        lastbrakestate = 10;
        digitalWrite(13, LOW);
        sensorValueBrake = analogRead(A1);
      }
    }
    if (sensorValueBrake >= 410 && sensorValueBrake <= 600 && lastbrakestate != 20) {
      Keyboard.release(KEYPAD_3);
      Keyboard.release(KEYPAD_9);
      Serial.println("Maintain brake");
      lastbrakestate = 20;
      sensorValueBrake = analogRead(A1);
    }
    if (sensorValueBrake >= 610 && sensorValueBrake <= 800 && lastbrakestate != 30) {
      if (lastbrakestate == 20)
      {
        if (sensorValueBrake >= 610 && sensorValueBrake <= 800) {
          Serial.println("Increase brake");
          digitalWrite(13, HIGH);
          Keyboard.press(KEYPAD_3);
          sensorValueBrake = analogRead(A1);
        }
        lastbrakestate = 30;
        digitalWrite(13, LOW);
        sensorValueBrake = analogRead(A1);
      }
      if (lastbrakestate == 40)
      {
        Keyboard.press(KEYPAD_9);
        if (sensorValueBrake >= 610 && sensorValueBrake <= 800) {
          Serial.println("Increase brake");
          digitalWrite(13, HIGH);
          Keyboard.press(KEYPAD_3);
          sensorValueBrake = analogRead(A1);
        }
        lastbrakestate = 30;
        digitalWrite(13, LOW);
        sensorValueBrake = analogRead(A1);
      }
    }
    if (sensorValueBrake >= 1000 && sensorValueBrake <= 1023 && lastbrakestate != 40) {
      if (lastbrakestate == 30)
      {
        Keyboard.release(KEYPAD_3);
        Keyboard.release(KEYPAD_9);
        Keyboard.press(KEYPAD_3);
        Serial.println("Full Brake");
        lastbrakestate = 40;
        sensorValueBrake = analogRead(A1);
      }
    }
  }
}
