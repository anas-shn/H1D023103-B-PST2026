#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

// Ganti alamat jika perlu (0x27 / 0x20)
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int pinPot = A0;

void setup() {
  Serial.begin(9600);

  // Inisialisasi LCD
  lcd.init();
  lcd.backlight();
}

void loop() {
  // Membaca nilai analog dari potensiometer
  int nilai = analogRead(pinPot);

  // Mapping nilai ADC (0–1023) ke panjang bar (0–16)
  int panjangBar = map(nilai, 0, 1023, 0, 16);

  // Tampilkan ke Serial Monitor
  Serial.print("Nilai ADC : ");
  Serial.println(nilai);

  // Baris 1: tampilkan nilai ADC
  lcd.setCursor(0, 0);
  lcd.print("ADC: ");
  lcd.print(nilai);
  lcd.print("    ");   // Menghapus sisa karakter lama

  // Baris 2: tampilkan bar grafik
  lcd.setCursor(0, 1);

  for (int i = 0; i < 16; i++) {
    if (i < panjangBar) {
      lcd.write(byte(255));   // Karakter blok penuh
    } else {
      lcd.print(" ");
    }
  }

  delay(200);
}
