#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

// Giriş pinleri
const int motorButton = 2;
const int seatbeltButton = 3;
const int doorSwitch = 4;
const int tempPin = A0;
const int ldrPin = A1;
const int fuelPin = A2;

// Çıkış pinleri
const int redLed = 22;
const int blueLed = 23;
const int yellowLed = 24;
const int rgbRed = 25;
const int rgbBlue = 26;
const int buzzer = 27;
const int motorEnable = 5;
const int motorIn1 = 28;
const int motorIn2 = 29;
const int fanEnable = 6;
const int fanIn1 = 30;
const int fanIn2 = 31;

// Durum değişkenleri
bool motorRunning = false;
bool fanRunning = false;
bool lastLDRState = false;
String lastMessage = "";
unsigned long lastDisplayTime = 0;
const unsigned long messageDuration = 3000;
int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

void setup() {
  pinMode(motorButton, INPUT_PULLUP);
  pinMode(seatbeltButton, INPUT_PULLUP);
  pinMode(doorSwitch, INPUT_PULLUP);

  pinMode(redLed, OUTPUT);
  pinMode(blueLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(rgbRed, OUTPUT);
  pinMode(rgbBlue, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(motorEnable, OUTPUT);
  pinMode(motorIn1, OUTPUT);
  pinMode(motorIn2, OUTPUT);
  pinMode(fanEnable, OUTPUT);
  pinMode(fanIn1, OUTPUT);
  pinMode(fanIn2, OUTPUT);

  lcd.begin(16, 2);
  lcd.clear();
  Serial.begin(9600);
}

void loop() {
  bool newMessageShown = false;

  // Kapı kontrolü
  if (digitalRead(doorSwitch) == HIGH) {
    digitalWrite(rgbRed, HIGH);
    digitalWrite(rgbBlue, HIGH);
    digitalWrite(motorEnable, LOW);
    digitalWrite(fanEnable, LOW);
    motorRunning = false;
    fanRunning = false;
    digitalWrite(redLed, LOW);
    digitalWrite(buzzer, LOW);
    showMessage("Uyari: Kapi Acik|Motor Calismaz!");
    return;
  } else {
    digitalWrite(rgbRed, LOW);
    digitalWrite(rgbBlue, LOW);
  }

  // Motor butonu kontrolü
  int buttonState = digitalRead(motorButton);
  if (buttonState != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (buttonState == LOW) {
      if (!motorRunning) {
        if (digitalRead(seatbeltButton) == HIGH) {
          digitalWrite(redLed, HIGH);
          digitalWrite(buzzer, HIGH);
          showMessage("Emniyet Kemeri|Takili Degil!");
        } else {
          digitalWrite(redLed, LOW);
          digitalWrite(buzzer, LOW);
          analogWrite(motorEnable, 255);
          digitalWrite(motorIn1, HIGH);
          digitalWrite(motorIn2, LOW);
          motorRunning = true;
          showMessage("Motor Calisiyor");
        }
      } else {
        analogWrite(motorEnable, 0);
        digitalWrite(motorIn1, LOW);
        digitalWrite(motorIn2, LOW);
        motorRunning = false;
        showMessage("Motor Durduruldu");
      }
      newMessageShown = true;
    }
  }
  lastButtonState = buttonState;

  // Emniyet kemeri kontrolü
  if (motorRunning && digitalRead(seatbeltButton) == HIGH) {
    analogWrite(motorEnable, 0);
    motorRunning = false;
    digitalWrite(redLed, HIGH);
    digitalWrite(buzzer, HIGH);
    showMessage("Emniyet Kemeri|Takili Degil!");
    newMessageShown = true;
  }

  // Sıcaklık kontrolü
  float temp = (analogRead(tempPin) * 5.0 / 1023.0) * 100.0;
  if (temp > 25 && !fanRunning) {
    analogWrite(fanEnable, 255);
    digitalWrite(fanIn1, HIGH);
    digitalWrite(fanIn2, LOW);
    fanRunning = true;
    showMessage("Sicaklik: " + String(temp, 1) + "C|Klima Acildi");
    newMessageShown = true;
  } else if (temp <= 25 && fanRunning) {
    analogWrite(fanEnable, 0);
    fanRunning = false;
    showMessage("Sicaklik: " + String(temp, 1) + "C");
    newMessageShown = true;
  }

  // LDR kontrolü
  int ldrValue = analogRead(ldrPin);
  bool currentLDRState = ldrValue < 250;
  Serial.print("LDR Degeri: "); Serial.println(ldrValue);
  if (currentLDRState != lastLDRState) {
    if (currentLDRState) {
      digitalWrite(blueLed, HIGH);
      showMessage("Farlar Acik");
    } else {
      digitalWrite(blueLed, LOW);
      showMessage("Farlar Kapandi");
    }
    lastLDRState = currentLDRState;
    newMessageShown = true;
  }

  // Yakıt kontrolü
  float fuelLevel = (analogRead(fuelPin) / 1023.0) * 100.0;
  if (fuelLevel <= 10 && fuelLevel > 5) {
    digitalWrite(yellowLed, HIGH);
    showMessage("Uyari: Yakit|Dusuk - %" + String(fuelLevel, 1));
    newMessageShown = true;
  } else if (fuelLevel <= 5 && fuelLevel > 0) {
    bool blinkState = millis() / 500 % 2;
    digitalWrite(yellowLed, blinkState);
    showMessage("Kritik: Yakit|Cok Az - %" + String(fuelLevel, 1));
    newMessageShown = true;
  } else if (fuelLevel <= 0) {
    digitalWrite(yellowLed, LOW);
    analogWrite(motorEnable, 0);
    analogWrite(fanEnable, 0);
    motorRunning = false;
    fanRunning = false;
    digitalWrite(redLed, LOW);
    digitalWrite(blueLed, LOW);
    digitalWrite(buzzer, LOW);
    showMessage("Yakit Bitti|Motor Durdu");
    newMessageShown = true;
  } else {
    digitalWrite(yellowLed, LOW);
  }

  // LCD temizleme süresi
  if (!newMessageShown && (millis() - lastDisplayTime > messageDuration) && lastMessage != "") {
    lcd.clear();
    lastMessage = "";
  }

  delay(50);
}

void showMessage(String message) {
  if (message != lastMessage && message != "") {
    lcd.clear();
    int separator = message.indexOf('|');
    if (separator != -1) {
      lcd.setCursor(0, 0);
      lcd.print(message.substring(0, separator));
      lcd.setCursor(0, 1);
      lcd.print(message.substring(separator + 1));
    } else {
      lcd.setCursor(0, 0);
      lcd.print(message);
    }
    lastMessage = message;
    lastDisplayTime = millis();
  }
}