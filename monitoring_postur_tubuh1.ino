#include <Wire.h>

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x21, 16, 2); 

// Pin definisi
const int trigPin = 2;
const int echoPin = 3;
const int buzzer = 4;
const int ledGreen = 5;
const int ledRed = 6;
const int buttonPin = 7;


long duration;
int distance;
int postureViolations = 0;
bool monitoring = false;
bool lastButtonState = HIGH;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();
  
  lcd.print("Monitor Postur");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Membaca status tombol
  bool buttonState = digitalRead(buttonPin);
  if (buttonState == LOW && lastButtonState == HIGH) {
    monitoring = !monitoring; // Toggle monitoring
    delay(200); // Debounce
  }
  lastButtonState = buttonState;

  if (monitoring) {
    lcd.setCursor(0, 0);
    lcd.print("Monitoring...");
    
    // Membaca jarak dari sensor ultrasonik
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034 / 2; // Menghitung jarak dalam cm

    // Logika posisi punggung
    if (distance > 30) { // Jarak > 30 cm dianggap postur buruk
      digitalWrite(buzzer, HIGH);
      digitalWrite(ledRed, HIGH);
      digitalWrite(ledGreen, LOW);

      lcd.setCursor(0, 1);
      lcd.print("Postur Buruk!  ");

      postureViolations++;
      delay(1000); // Tunggu 1 detik untuk menghindari penghitungan ganda
    } else { // Postur baik
      digitalWrite(buzzer, LOW);
      digitalWrite(ledRed, LOW);
      digitalWrite(ledGreen, HIGH);

      lcd.setCursor(0, 1);
      lcd.print("Postur Baik    ");
    }

    // Tampilkan jumlah pelanggaran
    lcd.setCursor(12, 0);
    lcd.print(":");
    lcd.print(postureViolations);
    lcd.print(" ");
  } else {
    // Sistem dalam mode mati
    lcd.setCursor(0, 0);
    lcd.print("Sistem Mati    ");
    lcd.setCursor(0, 1);
    lcd.print("Tekan Tombol   ");
    digitalWrite(buzzer, LOW);
    digitalWrite(ledRed, LOW);
    digitalWrite(ledGreen, LOW);
  }
}
