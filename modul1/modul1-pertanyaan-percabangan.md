# PRAKTIKUM PEMROGRAMAN SISTEM TERTANAM

## Pertanyaan Praktikum Modul 1: Percabangan

1. Pada kondisi apa program masuk ke blok if?
2. Pada kondisi apa program masuk ke blok else?
3. Apa fungsi dari perintah delay(timeDelay)?
4. Jika program yang dibuat memiliki alur mati → lambat → cepat → reset (mati),
   ubah menjadi LED tidak langsung reset → tetapi berubah dari cepat → sedang →
   mati

## Jawaban

### 1. Kondisi Masuk Blok if

Program akan masuk ke dalam blok if hanya jika ekspresi atau kondisi di dalam kurung bernilai benar (TRUE).

```ino
if (timeDelay <= 100)
```

Jadi, jika kondisinya adalah if `(timeDelay <= 100)`, maka program akan masuk ke dalam blok tersebut jika nilai variabel timeDelay adalah 100 atau angka apa pun yang lebih kecil dari 100 (misalnya 99, 50, atau 0).

### 2. Kondisi Masuk Blok else

Program akan masuk ke blok `else` jika kondisi pada `if` bernilai salah (FALSE) atau tidak terpenuhi.

### 3. fungsi delay(timeDelay)

Perintah delay() berfungsi untuk menghentikan sementara (pause) eksekusi program selama jangka waktu tertentu.

- Satuan: Parameter timeDelay diisi dengan angka dalam satuan milidetik (ms).
- Konversi: Karena 1 detik=1000 milidetik, maka jika kamu ingin berhenti selama 2 detik, kamu harus menulis delay(2000);.

### 4. Modifikasi Source Code

#### Kode Program

```ino
const int ledPin = 13;
int timeDelay = 200;     // Mulai dari cepat

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  digitalWrite(ledPin, HIGH);
  delay(timeDelay);

  digitalWrite(ledPin, LOW);
  delay(timeDelay);

  if (timeDelay == 200) {
    timeDelay = 500;
  }
  else if (timeDelay == 500) {
    timeDelay = 1000;
  }
  else {
    timeDelay = 0;
  }

  if (timeDelay == 0) {
    digitalWrite(ledPin, LOW);
    while(true);
  }
}
```

### Penjelasan Kode

- Menentukan pin LED pada Arduino.
- Mengatur kecepatan awal LED.
- Mengatur pin sebagai output.
- Menyalakan dan mematikan LED.
- Mengatur delay untuk kecepatan LED.
- Mengubah kecepatan dari cepat → sedang → lambat → mati.
- Menghentikan program secara permanen.
