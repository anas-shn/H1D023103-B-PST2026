# PRAKTIKUM PEMROGRAMAN SISTEM TERTANAM

## Pertanyaan Praktikum Modul 3: Inter-Integrated Circuit

1. Jelaskan bagaimana cara kerja komunikasi I2C antara Arduino dan LCD pada rangkaian tersebut!
2. Apakah pin potensiometer harus seperti itu? Jelaskan yang terjadi apabila pin kiri dan pin kanan tertukar!
3. Modifikasi program dengan menggabungkan antara UART dan I2C (keduanya sebagai output) sehingga:

- Data tidak hanya ditampilkan di LCD tetapi juga di Serial Monitor
- Adapun data yang ditampilkan pada Serial Monitor sesuai dengan table berikut:
  | ADC: 0 | Volt: 0.00 V | Persen: 0% |
  | :--- | :--- | :--- |
  - Tampilan jika potensiometer dalam kondisi diputar paling kiri
- ADC: 0 0% | setCursor(0, 0) dan Bar (level) | setCursor(0, 1)
- Berikan penjelasan disetiap baris kode nya dalam bentuk README.md!

4. Lengkapi table berikut berdasarkan pengamatan pada Serial Monitor:

| ADC | Volt (V) | Persen (%) |
| :-- | :------- | :--------- |
| 1   | 0.00 V   |            |
| 21  | 0.10 V   |            |
| 49  | 0.24 V   |            |
| 74  | 0.36 V   |            |
| 96  | 0.47 V   |            |

## Jawaban

### 1. Cara kerja komunikasi I2C antara Arduino dan LCD

Komunikasi I2C pada rangkaian ini memanfaatkan dua jalur utama, yaitu SDA dan SCL. Dalam sistem ini, Arduino berperan sebagai master, sedangkan LCD menjadi slave dengan alamat 0x27. Arduino terlebih dahulu membaca nilai dari potensiometer, kemudian mengirimkan data serta perintah ke LCD melalui library Wire dan LiquidCrystal_I2C dalam bentuk byte secara serial. Selanjutnya, modul I2C backpack seperti PCF8574 akan mengonversi data serial tersebut menjadi sinyal paralel, sehingga LCD dapat menampilkan nilai ADC dan bar secara langsung.

### 2. Apakah pin potensiometer harus seperti itu? Jelaskan yang terjadi apabila pin kiri dan pin kanan tertukar!

Tidak, karena penempatan GND di kiri dan VCC di kanan hanya memengaruhi arah perubahan nilai saja. Jika kedua pin itu ditukar, potensiometer tetap dapat berfungsi seperti biasa, namun hasil pembacaan ADC akan menjadi kebalikan. Artinya, ketika sebelumnya diputar ke arah tertentu nilai meningkat, kini justru akan menurun, karena perubahan tegangan pada pin tengah menjadi berlawanan (dari tinggi ke rendah atau sebaliknya).

### 3. Modifikasi program dengan menggabungkan antara UART dan I2C

#### Kode + Penjelasan

```ino
#include <Wire.h>              // Library komunikasi I2C
#include <LiquidCrystal_I2C.h> // Library LCD I2C
#include <Arduino.h>           // Library dasar Arduino

LiquidCrystal_I2C lcd(0x27, 16, 2); // Inisialisasi LCD (alamat 0x27, 16x2)
const int pinPot = A0;              // Pin potensiometer

void setup() {
  Serial.begin(9600); // Memulai komunikasi Serial (UART)
  lcd.init();         // Inisialisasi LCD
  lcd.backlight();    // Menyalakan lampu LCD
}

void loop() {
  int nilaiADC = analogRead(pinPot); // Membaca nilai analog (0–1023)

  float volt = nilaiADC * (5.0 / 1023.0); // Konversi ke tegangan (Volt)

  int persen = map(nilaiADC, 0, 1023, 0, 100); // Konversi ke persen (0–100%)

  int panjangBar = map(nilaiADC, 0, 1023, 0, 16); // Panjang bar LCD (0–16)

  // OUTPUT SERIAL
  Serial.print("| ADC: ");
  Serial.print(nilaiADC);        // Menampilkan nilai ADC

  Serial.print(" | Volt: ");
  Serial.print(volt, 2);         // Menampilkan tegangan (2 desimal)
  Serial.print("V");

  Serial.print(" | Persen: ");
  Serial.print(persen);          // Menampilkan persen
  Serial.println("% |");

  // OUTPUT LCD
  lcd.setCursor(0, 0);           // Baris 1
  lcd.print("ADC:");
  lcd.print(nilaiADC);           // Tampilkan ADC

  lcd.print(" ");
  lcd.print(persen);             // Tampilkan persen
  lcd.print("%   ");             // Hapus sisa karakter

  lcd.setCursor(0, 1);           // Baris 2 (bar grafik)
  for (int i = 0; i < 16; i++) {
    if (i < panjangBar)
      lcd.write(byte(255));      // Blok penuh (bar)
    else
      lcd.print(" ");            // Kosong
  }

  delay(200); // Delay agar pembacaan stabil
}
```

### 4. Lengkapi Tabel

| ADC | Volt (V) | Persen (%) |
| --- | -------- | ---------- |
| 0   | 0.00V    | 0%         |
| 20  | 0.10V    | 1%         |
| 41  | 0.20V    | 4%         |
| 61  | 0.30V    | 5%         |
| 82  | 0.40V    | 8%         |
