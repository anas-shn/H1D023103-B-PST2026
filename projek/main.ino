#include <Wire.h>
#include <Servo.h>

Servo myServo;
int systemState = 0; // 0:Idle, 1:Open, 2:Closing, 3:Denied, 4:Lockdown
int failCount = 0;
volatile bool dataReceived = false;
int receivedData[4];
const int correctPIN[4] = {6, 7, 8, 9};

const int p_pir = 2, p_buzzer = 3, p_red = 4, p_green = 5, p_yellow = 6, p_vib = A0;

void setup() {
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  Serial.begin(9600);
  myServo.attach(10); myServo.write(0);
  pinMode(p_pir, INPUT); pinMode(p_buzzer, OUTPUT);
  pinMode(p_red, OUTPUT); pinMode(p_green, OUTPUT); pinMode(p_yellow, OUTPUT);
  digitalWrite(p_red, HIGH);
  Serial.println(F("[BOOT] Security Kernel Active. Ready to validate."));
}

// Update pada loop() di Slave
void loop() {
  int vibLevel = analogRead(p_vib);

  // Cek jika sedang ada getaran (Breach)
  if (vibLevel > 850) {
    if (systemState != 5) {
      Serial.println(F("[ALARM] Physical Breach in Progress!"));
      systemState = 5; // Set state ke Breach
    }
    // Bunyi Buzzer selama getaran ada
    tone(p_buzzer, 1200);
  }
  else {
    // Jika sebelumnya sedang Breach dan sekarang getaran hilang
    if (systemState == 5) {
      noTone(p_buzzer);
      systemState = 0; // Kembali ke Normal
      Serial.println(F("[ALARM] Area Secured. G-Sensor Normalized."));
    }
  }

  // Tetap proses PIN jika tidak sedang Breach
  if (dataReceived && systemState != 5) {
    validate();
    dataReceived = false;
  }
}
void requestEvent() {
  byte packet[3] = {(byte)((digitalRead(p_pir) == HIGH) ? 1 : 0), (byte)systemState, (byte)failCount};
  Wire.write(packet, 3);
}

void validate() {
    Serial.print("Received: ");
    for (int i = 0; i < 4; i++) {
      Serial.print(receivedData[i]);
      Serial.print(" ");
    }
    Serial.println();
  bool match = true;
  for (int i = 0; i < 4; i++) if (receivedData[i] != correctPIN[i]) match = false;

  if (match) {
    executeOpen();
  } else {
    executeDenied();
  }
}

void executeOpen() {
  Serial.println(F("[AUTH] PIN Verified. Access Granted."));
  failCount = 0;
  for (int i = 0; i < 2; i++) { tone(p_buzzer, 1800, 100); delay(150); }

  systemState = 1; digitalWrite(p_red, LOW);
  for (int pos = 0; pos <= 90; pos += 5) {
    myServo.write(pos); digitalWrite(p_yellow, !digitalRead(p_yellow)); delay(50);
  }
  digitalWrite(p_yellow, LOW); digitalWrite(p_green, HIGH);

  delay(5000); // Sinkron dengan timer Master

  systemState = 2; digitalWrite(p_green, LOW);
  for (int pos = 90; pos >= 0; pos -= 5) {
    myServo.write(pos); digitalWrite(p_red, !digitalRead(p_red)); delay(50);
  }
  digitalWrite(p_red, HIGH); systemState = 0;
}

void executeDenied() {
  failCount++;
  systemState = 3;
  Serial.print(F("[AUTH] Incorrect PIN. Fails: ")); Serial.println(failCount);

  // Buzzer Pip Panjang 1x
  tone(p_buzzer, 200, 1500);
  delay(2000); // Beri waktu LCD Master tampilkan "Access Denied"

  if (failCount >= 3) {
    systemState = 4;
    Serial.println(F("[SECURITY] Threshold reached. Initiating Lockdown."));
    digitalWrite(p_red, HIGH);
    delay(10000); // 10s Lockdown
    failCount = 0;
  }
  systemState = 0;
}

void receiveEvent(int how) {
  for (int i = 0; i < 4; i++) if (Wire.available()) receivedData[i] = Wire.read();
  dataReceived = true;
}

void triggerAlarm() {
  Serial.println(F("[ALARM] Physical Breach Detected via Accelerometer!"));
  for (int i = 0; i < 5; i++) { tone(p_buzzer, 1000); delay(100); tone(p_buzzer, 600); delay(100); }
  noTone(p_buzzer);
}
