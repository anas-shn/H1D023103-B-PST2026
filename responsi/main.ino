#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

// LED
const int ledPin = 7;

// LCD
// Ganti alamat jika perlu (0x27 / 0x20)
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int pinPot = A0;

// Potensiometer

void setup() {
  // LED
  pinMode(ledPin, OUTPUT);

  // Inisialisasi LCD
  lcd.init();
  lcd.backlight();
}

void loop() {

  // Membaca nilai analog dari potensiometer
  int nilai = analogRead(pinPot);

  // Mapping nilai ADC (0–1023) ke panjang bar (0–16)
  int panjangBar = map(nilai, 0, 1023, 0, 16);

  // LED
  // tampilan LCD atas
  lcd.setCursor(0, 0);
  if( nilai < 306 ){
    digitalWrite(ledPin, HIGH);
    delay(200)
    lcd.print("Lampu: Menyala")
  } else {
    digitalWrite(ledPin, LOW);
    delay(200)
    lcd.print("Lampu: MATI KAU LAMPU")
  }

  // tampilan LCD bawah
  for (int i = 0; i < 16; i++) {
    if (i < panjangBar) {
      lcd.write(byte(255));   // Karakter blok penuh
    } else {
      lcd.print(" ");
    }
  }
  delay(200)
}
