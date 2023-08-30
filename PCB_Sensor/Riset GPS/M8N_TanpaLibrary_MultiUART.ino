#define AddGGA "$GNGGA"                                           // Data $GNGGA ($GPGGA) - lihat referensi (1)
                                                                  // Pengaturannya dengan U Center - lihat referensi (2)
#include <HardwareSerial.h>

HardwareSerial SerialPort1(1);                                    // Gunakan UART1, karena UART2 untuk PZEM-017

bool dat = false;
String DataOrg;
String DataP[14];

void setup() {
  Serial.begin(115200);
  SerialPort1.begin(9600, SERIAL_8N1, 4, 2);                      // Pin RX1 TX1
}

void loop() {
  if (SerialPort1.available() > 0) {
    if (dat == false) {
      char Add = (char)SerialPort1.read();
      DataOrg += Add;
      if (strcmp(DataOrg.c_str(), AddGGA) > 0) {
        dat = true;
        DataOrg = "";
      }
    }
    else {
      char buff = (char)SerialPort1.read();
      if (buff != '*') {
        DataOrg += buff;
      }
      if (buff == '\n') {
        for (byte i = 0; i < 14; i++) {
          DataP[i] = getValue(DataOrg, ',', i);
        }
        DataOrg = "";
        dat = false;
      }
    }
    Serial.println(Latitude(), 6);
    Serial.println(Longitude(), 6);
    Serial.println();
  }
}


float Latitude() {
  float lati = atof(DataP[1].c_str());                           // DataP[1] = Latitude, lihat referensi (1)
  int latit = lati / 100;
  double Lat = fmod(lati, 100);

  if (DataP[2].equals("S") == 1) {
    return (-1) * (latit + (Lat / 60));
  }
  else {
    return latit + (Lat / 60);
  }
}

float Longitude() {
  float llong = atof(DataP[3].c_str());                          // DataP[3] = Longitude, lihat referensi (1)
  int ilat = llong / 100;
  double mins = fmod(llong, 100);

  if (DataP[4].equals("W") == 1) {
    return (-1) * (ilat + (mins / 60));
  }
  else {
    return ilat + (mins / 60);
  }
}

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
