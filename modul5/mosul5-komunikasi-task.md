# Pertanyaan Komunikasi Task

## 1. Apakah kedua task berjalan secara bersamaan atau bergantian? Jelaskan mekanismenya!

Jawaban :
Task pada FreeRTOS sebenarnya berjalan secara bergantian karena mikrokontroler hanya memiliki satu CPU. Scheduler membagi waktu eksekusi setiap task menggunakan teknik time slicing dan prioritas. Ketika satu task sedang delay atau menunggu queue, scheduler akan menjalankan task lain sehingga proses terlihat berjalan paralel.

## 2. Apakah program ini berpotensi mengalami race condition? Jelaskan!

Jawaban :
Program ini tidak mengalami race condition karena pertukaran data antar task menggunakan queue dari FreeRTOS. Queue berfungsi sebagai mekanisme sinkronisasi sehingga akses data dilakukan secara teratur dan tidak saling bertabrakan. Selama task tidak memakai variabel global secara langsung tanpa pengamanan, proses eksekusi akan tetap aman dan stabil.

## 3. Modifikasilah program dengan menggunakan sensor DHT sesungguhnya sehingga informasi yang ditampilkan dinamis. Bagaimana hasilnya?

Jawaban :

```cpp
#include <Arduino_FreeRTOS.h>
#include <queue.h>
#include <DHT.h>

// ===================== KONFIGURASI DHT =====================
#define DHTPIN 2       // pin data DHT
#define DHTTYPE DHT11  // tipe sensor

DHT dht(DHTPIN, DHTTYPE);

// ===================== STRUKTUR DATA =====================
struct readings {
  float temp;
  float h;
};

// ===================== HANDLE QUEUE =====================
QueueHandle_t my_queue;

// ===================== DEKLARASI TASK =====================
void read_data(void *pvParameters);
void display(void *pvParameters);

void setup() {
  Serial.begin(9600);

  // inisialisasi sensor DHT
  dht.begin();

  // buat queue
  my_queue = xQueueCreate(1, sizeof(struct readings));

  // buat task
  xTaskCreate(read_data, "ReadSensors", 128, NULL, 1, NULL);
  xTaskCreate(display, "Display", 128, NULL, 1, NULL);
}

void loop() {
  // kosong karena menggunakan FreeRTOS
}

// ===================== TASK MEMBACA SENSOR =====================
void read_data(void *pvParameters) {
  struct readings x;

  for (;;) {

    // baca data suhu dan kelembapan
    x.temp = dht.readTemperature();
    x.h = dht.readHumidity();

    // kirim data ke queue
    xQueueSend(my_queue, &x, portMAX_DELAY);

    // delay 2 detik
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

// ===================== TASK MENAMPILKAN DATA =====================
void display(void *pvParameters) {
  struct readings x;

  for (;;) {

    // terima data dari queue
    if (xQueueReceive(my_queue, &x, portMAX_DELAY) == pdPASS) {

      Serial.print("Temperature = ");
      Serial.print(x.temp);
      Serial.println(" C");

      Serial.print("Humidity = ");
      Serial.print(x.h);
      Serial.println(" %");

      Serial.println("-------------------");
    }
  }
}
```

Setelah program dimodifikasi menggunakan sensor DHT asli, nilai suhu dan kelembapan yang tampil di Serial Monitor menjadi dinamis sesuai kondisi lingkungan sekitar. Jika suhu ruangan berubah atau sensor terkena hembusan udara, maka data temperature dan humidity juga ikut berubah secara real-time. Task read_data bertugas membaca data sensor lalu mengirimkannya melalui queue, sedangkan task display menerima dan menampilkan data tersebut ke Serial Monitor secara terpisah sehingga program tetap berjalan stabil menggunakan FreeRTOS.
