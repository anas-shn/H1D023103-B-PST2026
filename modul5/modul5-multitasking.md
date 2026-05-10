# Pertanyaan percobaan Multitasking

## 1. Apakah ketiga task berjalan secara bersamaan atau bergantian? Jelaskan mekanismenya!

Jawaban :
Ketiga task sebenarnya tidak berjalan secara bersamaan, melainkan dijalankan bergantian oleh scheduler pada FreeRTOS. Scheduler membagi waktu eksekusi setiap task secara cepat dengan prioritas yang sama, sehingga terlihat seperti paralel. Ketika suatu task menjalankan `vTaskDelay()`, task tersebut akan masuk ke kondisi blocked dan CPU langsung diberikan ke task lain untuk dieksekusi.

## 2. Bagaimana cara menambahkan task keempat? Jelaskan langkahnya!

Jawaban :
Untuk menambahkan task baru, buat fungsi seperti `void Task4(void *pvParameters)` lalu isi proses yang ingin dijalankan di dalam `while(1)`. Setelah itu, tambahkan task tersebut pada `setup()` menggunakan `xTaskCreate()` dengan menentukan fungsi task, nama task, ukuran stack, prioritas, dan parameter lainnya. Perlu diperhatikan agar tidak terjadi konflik penggunaan resource, misalnya pin atau `Serial`, supaya seluruh task dapat berjalan dengan baik dan stabil.

## 3. Modifikasilah program dengan menambah sensor (misalnya potensiometer), lalu gunakan nilainya untuk mengontrol kecepatan LED! Bagaimana hasilnya?

Jawaban :

```cpp

#include <Arduino_FreeRTOS.h>

// deklarasi task
void TaskBlink1(void *pvParameters);
void TaskBlink2(void *pvParameters);
void TaskPrint(void *pvParameters);

// pin potensiometer
const int potPin = A0;

int delayLed = 200;

void setup() {

  Serial.begin(9600);

  xTaskCreate(TaskBlink1, "Task1", 128, NULL, 1, NULL);
  xTaskCreate(TaskBlink2, "Task2", 128, NULL, 1, NULL);
  xTaskCreate(TaskPrint, "Task3", 128, NULL, 1, NULL);

  vTaskStartScheduler();
}

void loop() {
}

// ================= TASK 1 =================
void TaskBlink1(void *pvParameters) {

  pinMode(8, OUTPUT);

  while (1) {

    // baca potensiometer
    int nilaiADC = analogRead(potPin);

    // ubah nilai ADC menjadi delay 50–1000 ms
    delayLed = map(nilaiADC, 0, 1023, 50, 1000);

    Serial.print("Delay LED: ");
    Serial.println(delayLed);

    digitalWrite(8, HIGH);
    vTaskDelay(delayLed / portTICK_PERIOD_MS);

    digitalWrite(8, LOW);
    vTaskDelay(delayLed / portTICK_PERIOD_MS);
  }
}

// ================= TASK 2 =================
void TaskBlink2(void *pvParameters) {

  pinMode(7, OUTPUT);

  while (1) {

    Serial.println("Task2");

    digitalWrite(7, HIGH);
    vTaskDelay(300 / portTICK_PERIOD_MS);

    digitalWrite(7, LOW);
    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
}

// ================= TASK 3 =================
void TaskPrint(void *pvParameters) {

  int counter = 0;

  while (1) {

    counter++;
    Serial.println(counter);

    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}
```

Hasilnya potensiometer berhasil digunakan untuk mengatur kecepatan kedipan LED pada pin 8. Saat potensiometer diputar ke nilai kecil, LED berkedip lebih cepat karena delay menjadi kecil. Sebaliknya, ketika potensiometer diputar ke nilai besar, LED berkedip lebih lambat karena delay bertambah besar. Task lain tetap berjalan normal karena FreeRTOS membagi waktu eksekusi setiap task secara bergantian.
