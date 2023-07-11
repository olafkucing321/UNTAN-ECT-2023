#include <TinyGPS++.h>
#include <SoftwareSerial.h>

static const int RXPin = 16, TXPin = 17;
static const uint32_t GPSBaud = 9600;

TinyGPSPlus gps;
SoftwareSerial gpsneo(RXPin, TXPin);

void setup() {
  Serial.begin(115200);
  gpsneo.begin(GPSBaud);
  Serial.println(F("GPS menggunakan ESP32"));
  Serial.println();
}

void tampilkandata() {
  Serial.print(F("Lokasi (Lat, Lng): "));
  if (gps.location.isValid()) {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else {
    Serial.print(F("Gagal"));
  }

  //Menampilkan data tanggal
  Serial.print(F("  Tanggal: "));
  if (gps.date.isValid()) {
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else {
    Serial.print(F("Gagal"));
  }

  //Menampilkan data waktu
  Serial.print(F(" "));
  if (gps.time.isValid()) {
    if (gps.time.hour() < 10) Serial.print(F(""));
    Serial.print(gps.time.hour() + 7);
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    //    Serial.print(F("."));
    //    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    //    Serial.print(gps.time.centisecond());
  }
  else {
    Serial.print(F("Gagal"));
  }
  Serial.println();
}

void loop() {
  while (gpsneo.available() > 0)
    if (gps.encode(gpsneo.read()))
      tampilkandata();

  if (millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println(F("Module GPS tidak terdeteksi, cek wiring!"));
    while (true);
  }
}

// Sekitaran 3 menit baru dapat data
