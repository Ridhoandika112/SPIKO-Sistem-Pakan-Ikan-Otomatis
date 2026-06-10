#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <RTClib.h>
#include <Servo.h>
#include <ArduinoJson.h> 

// ================= PIN CONFIG =================
#define ONE_WIRE_BUS 22     // DS18B20 DQ ke pin D22
#define PH_PIN A0           // Potensiometer pH ke A0
#define SERVO_PIN 9         // Servo ke PWM D9

// ================= OBJECT =================
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature suhuSensor(&oneWire);
RTC_DS1307 rtc;
Servo servoPakan;

// ================= VARIABEL =================
float suhuAir;
float nilaiPH;

int jamPakan1 = 6;      
int menitPakan1 = 0;

int jamPakan2 = 18;     
int menitPakan2 = 0;

bool sudahPakan1 = false;
bool sudahPakan2 = false;

unsigned long waktuSebelumnya = 0;
const long intervalKirim = 2000;

// ================= FUNGSI BACA PH =================
float bacaPH() {
  int adcValue = analogRead(PH_PIN);
  float ph = map(adcValue, 0, 1023, 0, 1400) / 100.0;
  return ph;
}

// ================= FUNGSI BERI PAKAN =================
void beriPakan() {
  DateTime now = rtc.now();
  char timeBuffer[10];
  sprintf(timeBuffer, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());

  // Kirim status katup dibuka ke bridge via Serial (COM3)
  StaticJsonDocument<100> docBuka;
  docBuka["status"] = "Katup Dibuka";
  docBuka["waktu"] = timeBuffer;
  serializeJson(docBuka, Serial);
  Serial.println();

  servoPakan.write(90);
  delay(3000);
  servoPakan.write(0);

  // Kirim status katup tertutup ke bridge via Serial (COM3)
  StaticJsonDocument<100> docTutup;
  docTutup["status"] = "Katup Tertutup";
  docTutup["waktu"] = timeBuffer;
  serializeJson(docTutup, Serial);
  Serial.println();
}

// ================= SETUP =================
void setup() {
  Serial.begin(9600);

  suhuSensor.begin();
  Wire.begin();

  servoPakan.attach(SERVO_PIN);
  servoPakan.write(0);

  if (!rtc.begin()) {
    Serial.println("{\"error\": \"RTC tidak terdeteksi!\"}");
    while (1);
  }

  // Selalu sync waktu RTC dengan waktu komputer saat upload
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

// ================= LOOP =================
void loop() {
  DateTime now = rtc.now();
  unsigned long waktuSekarang = millis();

  // 1. Terima perintah dari bridge via Serial (COM3)
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    if (input == "FEED_MANUAL") {
      beriPakan();
    }
  }

  // 2. Jadwal pakan otomatis
  if (now.hour() == jamPakan1 && now.minute() == menitPakan1 && !sudahPakan1) {
    beriPakan();
    sudahPakan1 = true;
  }
  if (now.hour() == jamPakan2 && now.minute() == menitPakan2 && !sudahPakan2) {
    beriPakan();
    sudahPakan2 = true;
  }
  if (now.hour() == 0 && now.minute() == 0) {
    sudahPakan1 = false;
    sudahPakan2 = false;
  }

  // 3. Kirim data sensor setiap 2 detik
  if (waktuSekarang - waktuSebelumnya >= intervalKirim) {
    waktuSebelumnya = waktuSekarang;

    DateTime nowKirim = rtc.now();
    suhuSensor.requestTemperatures();
    suhuAir = suhuSensor.getTempCByIndex(0);
    nilaiPH = bacaPH();

    StaticJsonDocument<200> doc;
    char timeBuffer[10];
    sprintf(timeBuffer, "%02d:%02d:%02d", nowKirim.hour(), nowKirim.minute(), nowKirim.second());
    doc["waktu"] = timeBuffer;
    doc["suhu"] = suhuAir;
    doc["ph"] = nilaiPH;

    serializeJson(doc, Serial);
    Serial.println();
  }
}
