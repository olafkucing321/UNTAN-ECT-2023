#define AddGGA "$GNGGA"                                           // Data $GNGGA ($GPGGA) - lihat referensi (1)
                                                                  // Pengaturannya dengan U Center - lihat referensi (2)
#include <HardwareSerial.h>

HardwareSerial SerialPort1(1);                                    // Gunakan UART1, karena UART2 untuk PZEM-017

String DataOrg;
String DataP[14];

void setup() {
  Serial.begin(115200);
  SerialPort1.begin(9600, SERIAL_8N1, 4, 2);                      // Pin RX1 TX1  --  M8N Hijau - 4 || Kuning - 2
}

void loop() {
  if (SerialPort1.available() > 0) {
    DataOrg = SerialPort1.readStringUntil('\n');
    String AddCheck = getValue(DataOrg, ',', 0);
    if (AddCheck.equals(AddGGA) == 1) {
      for (byte i = 1; i <= 14; i++) {
        DataP[i-1] = getValue(DataOrg, ',', i);
      }
    }
    Serial.print(Jam());
    Serial.print(":");
    Serial.print(Menit());
    Serial.print(":");
    Serial.println(Detik());
    Serial.println(Latitude(), 6);
    Serial.println(Longitude(), 6);
    Serial.println(Satelit());
    Serial.println(sumData());
    Serial.println();
  }
}

float Latitude() {
  float lati = atof(DataP[1].c_str());                                          // DataP[1] = Latitude, Lihat referensi 1
  int latit = lati / 100;
  double Lat = fmod(lati, 100);
  double latitude = latit + (Lat / 60.0f);
  return (DataP[2].equals("S") == 1) ? (-1.0f) * latitude : latitude;
}

float Longitude() {
  float llong = atof(DataP[3].c_str());                                          // DataP[3] = Longitude, Lihat referensi 1
  int ilat = llong / 100;
  double mins = fmod(llong, 100);
  double longitude = ilat + (mins / 60.0f);
  return (DataP[4].equals("W") == 1) ? (-1.0f) * longitude : longitude;
}

//////////////////////// Jam, menit, detik, Satelit, dan sumData tidak terlalu diperlukan

byte Detik() {
  return (int)atof(DataP[0].c_str()) % 100;
}

byte Menit() {
  return (int)(atof(DataP[0].c_str()) / 100) % 100;
}

byte Jam() {
  byte jam = (atof(DataP[0].c_str()) / 10000) + 7;
  return (jam > 23) ? jam -= 24 : jam;
}

byte Satelit() {
  return atof(DataP[6].c_str());
}

int sumData() {
  char val[3];
  DataP[13].toCharArray(val, 4);
  String sum = String(val[1]) + String(val[2]);
  return strtol(sum.c_str(), NULL, 16);
}

/////////////////////////////////////////////////////////////

String getValue(String data, char separator, int index) {        // Pemisah data berdasarkan koma, karena data dari M8N dipisah koma
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

/* Referensi
   (1) https://docs.arduino.cc/learn/communication/gps-nmea-data-101#arduino-and-the-gps-nmea-0183-messaging-protocol
   (2) https://www.youtube.com/watch?v=ZaDpx9DvCPo
*/
