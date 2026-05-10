# PRAKTIKUM PEMROGRAMAN SISTEM TERTANAM

## Pertanyaan Praktikum Modul 3: Komunikasi Serial

1. Jelaskan proses dari input keyboard hingga LED menyala/mati!
2. Mengapa digunakan Serial.available() sebelum membaca data? Apa yang terjadi jika
   baris tersebut dihilangkan?
3. Modifikasi program agar LED berkedip (blink) ketika menerima input '2' dengan kondisi
   jika ‘2’ aktif maka LED akan terus berkedip sampai perintah selanjutnya diberikan dan
   berikan penjelasan disetiap baris kode nya dalam bentuk README.md!
4. Tentukan apakah menggunakan delay() atau milis()! Jelaskan pengaruhnya terhadap
   sistem

## Jawaban

### 1) Jelaskan proses dari input keyboard hingga LED menyala/mati!

**Jawaban :**  
Program dimulai dengan menginisialisasi komunikasi serial menggunakan Serial.begin(9600). Selanjutnya, pin digital yang terhubung dengan LED disetting sebagai output. Arduino kemudian memeriksa apakah terdapat data yang masuk dari Serial Monitor melalui Serial.available(), dan jika ada, data tersebut akan dibaca menggunakan Serial.read(). Apabila input yang diterima adalah ‘1’, maka LED akan menyala, sedangkan jika input ‘0’, LED akan dimatikan. Jika input yang diberikan tidak sesuai, program akan menampilkan pesan error pada Serial Monitor. Seluruh rangkaian proses ini berjalan secara terus-menerus di dalam loop.

---

### 2) Mengapa digunakan Serial.available() sebelum membaca data? Apa yang terjadi jika baris tersebut dihilangkan?

**Jawaban :**  
Serial.available() berfungsi untuk mengecek apakah data sudah masuk ke buffer penerima pada perangkat seperti Arduino sebelum dilakukan proses pembacaan. Hal ini penting karena pengiriman data secara serial tidak terjadi secara langsung, melainkan membutuhkan waktu. Tanpa pengecekan ini, program akan langsung menjalankan Serial.read() meskipun data belum sepenuhnya diterima, sehingga nilai yang terbaca sering kali menjadi -1 atau karakter kosong. Dengan adanya pengecekan menggunakan Serial.available(), program dapat memastikan bahwa hanya data yang valid yang diproses, sehingga menghindari kesalahan logika maupun munculnya output yang tidak sesuai pada Serial Monitor.

---

### 3) Modifikasi program agar LED berkedip (blink) ketika menerima input '2'Dengan kondisi jika ‘2’ aktif maka LED akan terus berkedip sampai perintah selanjutnya diberikan dan berikan penjelasan disetiap baris kode nya dalam bentuk README.md!

**Jawaban :**

#### Program Kontrol LED dengan Serial (ON, OFF, dan BLINK)

Program ini digunakan untuk mengontrol LED menggunakan input dari Serial Monitor:

- '1' → LED menyala
- '0' → LED mati
- '2' → LED berkedip (blink terus sampai ada perintah baru)

---

### Kode Program

```ino
#include <Arduino.h>

// Menentukan pin LED
const int PIN_LED = 12;

// Variabel untuk menyimpan mode LED
int mode = 0;
// 0 = mati, 1 = nyala, 2 = blink

// Variabel untuk blink tanpa delay blocking
unsigned long previousMillis = 0;
const long interval = 500; // interval kedip (ms)
bool ledState = LOW;

void setup() {
  Serial.begin(9600); // Memulai komunikasi serial
  Serial.println("Ketik '1' untuk ON, '0' untuk OFF, '2' untuk BLINK");

  pinMode(PIN_LED, OUTPUT); // Mengatur pin LED sebagai output
}

void loop() {

  // Mengecek apakah ada data masuk dari Serial Monitor
  if (Serial.available() > 0) {

    char data = Serial.read(); // Membaca 1 karakter input

    if (data == '1') {
      mode = 1; // Mode nyala
      digitalWrite(PIN_LED, HIGH); // LED langsung menyala
      Serial.println("LED ON");
    }
    else if (data == '0') {
      mode = 0; // Mode mati
      digitalWrite(PIN_LED, LOW); // LED langsung mati
      Serial.println("LED OFF");
    }
    else if (data == '2') {
      mode = 2; // Mode blink
      Serial.println("LED BLINK");
    }
    else if (data != '\n' && data != '\r') {
      Serial.println("Perintah tidak dikenal");
    }
  }

  // Logika untuk mode BLINK
  if (mode == 2) {
    unsigned long currentMillis = millis(); // Ambil waktu sekarang

    // Cek apakah sudah melewati interval
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;

      // Toggle LED (nyala ↔ mati)
      ledState = !ledState;
      digitalWrite(PIN_LED, ledState);
    }
  }
}

```

---

### 4) Tentukan apakah menggunakan delay() atau millis()! Jelaskan pengaruhnya terhadap sistem

Dengan kondisi jika ‘2’ aktif maka LED akan terus berkedip sampai perintah selanjutnya diberikan dan berikan penjelasan disetiap baris kode nya dalam bentuk README.md!

**Jawaban :**
Gunakan millis() apabila ingin program tetap berjalan tanpa menghambat proses lain, sehingga Arduino masih bisa membaca input dari Serial secara langsung. Berbeda dengan delay() yang akan menghentikan sementara eksekusi program selama waktu tertentu, penggunaan millis() memungkinkan LED tetap berkedip tanpa membuat sistem menjadi tidak responsif. Dengan cara ini, perubahan perintah dari user dapat segera diproses tanpa harus menunggu jeda selesai.

![Rangkaian Skematik Komunikasi Serial](img/komunikasi-serial.jpg)
