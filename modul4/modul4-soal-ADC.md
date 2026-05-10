# Pertanyaan Praktikum

## 1. Apa fungsi perintah analogRead() pada rangkaian praktikum ini?

**Jawab:**
fungsi `analogRead()` digunakan untuk membaca sinyal analog dari potensiometer dan mengubahnya menjadi data digital menggunakan ADC pada mikrokontroler. Tegangan 0–5V dikonversi menjadi nilai 0–1023. Nilai ini kemudian dipakai sebagai input untuk mengatur sudut gerak servo sesuai posisi potensiometer.

## 2. Mengapa diperlukan fungsi map() dalam program tersebut?

**Jawab:**
Fungsi `map()` digunakan untuk menyesuaikan dua rentang nilai yang berbeda, yaitu mengubah hasil pembacaan potensiometer (0–1023) menjadi sudut servo (0–180 derajat). Dengan begitu, pergerakan servo tetap sesuai batasnya dan berubah secara proporsional mengikuti posisi potensiometer.

## 3. Modifikasi program berikut agar servo hanya bergerak dalam rentang 30° hingga 150°, meskipun potensiometer tetap memiliki rentang ADC 0–1023.

**Jawab:**

```cpp
#include <Servo.h>

Servo myservo;

// ===================== PIN SETUP =====================
const int potensioPin = A0;
const int servoPin = 9;

// ===================== VARIABEL =====================
int pos = 0;
int val = 0;

void setup() {

  myservo.attach(servoPin);
  Serial.begin(9600);

}

void loop() {

  // ===================== BACA POTENSIO =====================
  val = analogRead(potensioPin);

  // ===================== MAP KE SERVO (30°–150°) =====================
  pos = map(val,
             0,
             1023,
             30,   // sudut minimum baru
             150); // sudut maksimum baru

  // ===================== GERAKKAN SERVO =====================
  myservo.write(pos);

  // ===================== SERIAL MONITOR =====================
  Serial.print("ADC Potensio: ");
  Serial.print(val);
  Serial.print(" | Sudut Servo: ");
  Serial.println(pos);

  delay(15);
}
```

Program ini membaca nilai dari potensiometer melalui pin analog, lalu mengubahnya menjadi sudut servo menggunakan fungsi map(), tetapi dibatasi hanya pada rentang 30° hingga 150°. Artinya, meskipun potensiometer memiliki nilai penuh 0–1023, pergerakan servo tidak akan melewati batas tersebut. Servo kemudian digerakkan sesuai hasil konversi, dan nilai ADC serta sudut servo ditampilkan ke Serial Monitor untuk pemantauan.
