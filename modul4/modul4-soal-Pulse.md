# Pertanyaan Praktikum

## 1. Jelaskan mengapa LED dapat diatur kecerahannya menggunakan fungsi analogWrite()!

**Jawab:**
`analogWrite()` mengatur kecerahan LED dengan teknik PWM, yaitu menyalakan dan mematikan arus sangat cepat sehingga terlihat seperti cahaya yang berubah-ubah. Nilai 0–255 menentukan lamanya LED menyala dibanding mati (duty cycle). Nilai kecil membuat LED redup, sedangkan nilai besar membuat LED semakin terang.

## 2. Apa hubungan antara nilai ADC (0–1023) dan nilai PWM (0–255)?

**Jawab:**
Nilai ADC (0–1023) dan PWM (0–255) punya hubungan linier, tapi beda resolusi: ADC 10-bit, PWM 8-bit. Karena nilai ADC 4× lebih besar, biasanya dibagi 4 supaya sesuai ke PWM. Jadi, nilai maksimum ADC (1023) akan jadi PWM 255, sehingga input seperti potensiometer bisa mengatur LED atau motor dengan halus.

## 3. Modifikasilah program berikut agar LED hanya menyala pada rentang kecerahan sedang, yaitu hanya ketika nilai PWM berada pada rentang 50 sampai 200.

**Jawab:**

```cpp
#include <Servo.h>

Servo myservo;

// ===================== PIN SETUP =====================
const int potensioPin = A0;
const int servoPin = 9;
const int ledPin = 10; // pin LED (PWM)

// ===================== VARIABEL =====================
int pos = 0;
int val = 0;
int pwmVal = 0; // nilai PWM

void setup() {

  myservo.attach(servoPin);
  pinMode(ledPin, OUTPUT); // set LED sebagai output

  Serial.begin(9600);
}

void loop() {

  // ===================== BACA POTENSIO =====================
  val = analogRead(potensioPin);

  // ===================== MAP KE SERVO =====================
  pos = map(val, 0, 1023, 0, 180);
  myservo.write(pos);

  // ===================== MAP KE PWM =====================
  pwmVal = map(val, 0, 1023, 0, 255);

  // ===================== LOGIKA LED =====================
  if (pwmVal >= 50 && pwmVal <= 200) {
    analogWrite(ledPin, pwmVal); // LED nyala (kecerahan sedang)
  } else {
    analogWrite(ledPin, 0); // LED mati
  }

  // ===================== SERIAL MONITOR =====================
  Serial.print("ADC: ");
  Serial.print(val);
  Serial.print(" | PWM: ");
  Serial.print(pwmVal);
  Serial.print(" | Servo: ");
  Serial.println(pos);

  delay(15);
}
```

Program ini membaca nilai dari potensiometer melalui pin analog, lalu mengubahnya menjadi dua output: sudut servo (0–180 derajat) dan nilai PWM (0–255). Servo akan bergerak mengikuti perubahan potensiometer, sedangkan LED hanya akan menyala jika nilai PWM berada di rentang 50–200 (kecerahan sedang), dan akan mati jika di luar rentang tersebut. Selain itu, program juga menampilkan nilai ADC, PWM, dan sudut servo ke Serial Monitor untuk memudahkan pemantauan.
