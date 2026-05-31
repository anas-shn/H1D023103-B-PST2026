#include <Servo.h>

// =====================
// Definisi PIN
// =====================
const int PIN_BUTTON     = 2;   // interrupt
const int PIN_LED_HIJAU  = 4;
const int PIN_LED_MERAH  = 5;
const int TRIGPIN        = 6;
const int ECHOPIN        = 7;
const int PIN_SERVO      = 9;

// =====================
// Konfigurasi
// =====================
const int JARAK_ADA_OBJEK    = 20;
const int SUDUT_BUKA         = 90;
const int SUDUT_TUTUP        = 0;
// Konfigurasi keamanan (tidak digunakan - logika disederhanakan)

// =====================
// Variabel
// =====================
Servo palang;
long timer;
bool palangTerbuka = false;
unsigned long waktuBuka = 0;
const unsigned long DELAY_TUTUP = 30000;  // 30 detik

volatile bool flagTombol = false;
volatile unsigned long lastInterruptTime = 0;
const unsigned long debounceDelay = 300;

// Variabel keamanan
bool tombolDikunci = false;           // mencegah tombol aktif saat palang terbuka

// =====================
// ISR tombol
// =====================
void tekanTombol() {
  unsigned long sekarang = millis();
  if (sekarang - lastInterruptTime > debounceDelay) {
    flagTombol = true;
    lastInterruptTime = sekarang;
  }
}

// =====================
// Fungsi sensor
// =====================
int bacaJarak() {
  digitalWrite(TRIGPIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGPIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN, LOW);

  timer = pulseIn(ECHOPIN, HIGH, 30000);
  if (timer == 0) return 999;
  return timer / 58;
}

void bukaPalang() {
  palang.write(SUDUT_BUKA);
  digitalWrite(PIN_LED_HIJAU, HIGH);
  digitalWrite(PIN_LED_MERAH, LOW);
  palangTerbuka = true;
  tombolDikunci = true;  // kunci tombol
  waktuBuka = millis();
  Serial.println("Palang dibuka");
}

void tutupPalang() {
  palang.write(SUDUT_TUTUP);
  digitalWrite(PIN_LED_HIJAU, LOW);
  digitalWrite(PIN_LED_MERAH, HIGH);
  palangTerbuka = false;
  tombolDikunci = false;  // buka kunci tombol
  Serial.println("Palang ditutup");
}

// =====================
// Setup
// =====================
void setup() {
  Serial.begin(9600);

  pinMode(TRIGPIN, OUTPUT);
  pinMode(ECHOPIN, INPUT);
  pinMode(PIN_LED_HIJAU, OUTPUT);
  pinMode(PIN_LED_MERAH, OUTPUT);
  pinMode(PIN_BUTTON, INPUT_PULLUP);

  palang.attach(PIN_SERVO);
  tutupPalang();

  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON), tekanTombol, FALLING);

  Serial.println("Sistem parkir siap");
}

// =====================
// Loop
// =====================
void loop() {
  int jarak = bacaJarak();

  if (!palangTerbuka) {
    if (flagTombol) {
      flagTombol = false;
      Serial.println("Tombol ditekan");
      bukaPalang();
    }
  } else {
    // Palang terbuka - cek sensor ultrasonik
    if (jarak == 999 || jarak > JARAK_ADA_OBJEK) {
      // Tidak ada objek terdeteksi
      if (millis() - waktuBuka >= DELAY_TUTUP) {
        // Sudah 30 detik, tutup palang
        delay(500);
        tutupPalang();
      }
    } else {
      // Ada objek terdeteksi, reset waktu
      waktuBuka = millis();
    }
  }

  delay(50);
}
