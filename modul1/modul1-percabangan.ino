const int ledPin = 2;
int timeDelay = 1000;

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // 1. Jalankan siklus kedip (Nyalakan & Matikan)
  digitalWrite(ledPin, HIGH);
  delay(timeDelay);

  digitalWrite(ledPin, LOW);
  delay(timeDelay);

  // 2. Logika perubahan delay harus di dalam loop()
  if (timeDelay <= 100) {
    delay(3000);         // Jeda 3 detik sebelum reset
    timeDelay = 1000;    // Reset ke kecepatan awal
  } else {
    timeDelay -= 100;    // Kurangi delay agar kedipan makin cepat
  }
}
