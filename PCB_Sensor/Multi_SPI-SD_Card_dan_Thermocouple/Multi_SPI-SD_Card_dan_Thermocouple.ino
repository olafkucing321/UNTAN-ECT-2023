#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "max6675.h"

#define CS_PIN 5 //Mendefinisikan variabel CS_PIN pada pin GPIO5 di ESP32
int thermoDO = 12;
int thermoCS = 15;
int thermoCLK = 14;

File dataFile;
MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

unsigned long waktuSebelumnya_VSPI = 0;
unsigned long waktuSebelumnya_HSPI = 0;
float V, I, P, T, v, E = 0;

void setup() {
  Serial.begin(115200);   //Serial komunikasi untuk dihubungkan ke laptop/PC dengan baud rate 115200
  delay(1000);

  Serial.println("Inisialisasi kartu.");
  if (!SD.begin(CS_PIN)) {
    Serial.println("Inisialisasi Gagal");
    return;
  }
  Serial.println();
  Serial.println("Inisialisasi Berhasil.\n");

  dataFile = SD.open("/KAPUAS_3.XLS", FILE_WRITE); //Menjadikan variabel dataFile sebagai alat untuk membuat file dan membuka file di SD card
  if (!dataFile) {
    Serial.println("Gagal membuka file untuk ditulis");
    return;
  }

  if (dataFile) {
    Serial.println("Berhasil membuka file untuk ditulis");
    dataFile.print("Tegangan");
    dataFile.print("\t"); //Tab 1 kali
    dataFile.print("Arus");
    dataFile.print("\t"); //Tab 1 kali
    dataFile.print("Daya");
    dataFile.print("\t"); //Tab 1 kali
    dataFile.print("Temperature");
    dataFile.print("\t"); //Tab 1 kali
    dataFile.print("Kecepatan");
    dataFile.print("\t"); //Tab 1 kali
    dataFile.print("Energi");
    //menutup file setelah menggunakannya
    dataFile.close();
  }
  // jika file tidak dapat dibuka, data tidak akan ditulis.
  else {
    Serial.println("Gagal membuka file KAPUAS_3.XLS"); //Menampilkan kata "Gagal membuka  file NilaiPot.xls" diserial monitor laptop
  }
  delay(1000);
}


void loop() {
  if (millis() - waktuSebelumnya_VSPI >= 500) {
    waktuSebelumnya_VSPI = millis();
    V = 50;
    I = random(0, 20);
    P = V * I;
    T = random(30, 50);
    v++;
    E = P * millis() / 1000;

    SD_Card();
  }

  if (millis() - waktuSebelumnya_HSPI >= 500) {
    waktuSebelumnya_HSPI = millis();
    Serial.print("C = ");
    Serial.println(thermocouple.readCelsius());
    Serial.print("F = ");
    Serial.println(thermocouple.readFahrenheit());
  }
}

void SD_Card() {
  dataFile = SD.open("/KAPUAS_3.XLS", FILE_APPEND); //Menjadikan variabel dataFile sebagai alat untuk membuat file dan membuka file di SD card
  if (!dataFile) {
    Serial.println("Gagal membuka file untuk menambahkan data");
    return;
  }
  if (dataFile) {
    Serial.println("Membuka file untuk menambahkan data");
    dataFile.print("\n");
    dataFile.print(String(V));
    dataFile.print("\t"); //Tab 1 kali
    dataFile.print(String(I));
    dataFile.print("\t"); //Tab 1 kali
    dataFile.print(String(P));
    dataFile.print("\t"); //Tab 1 kali
    dataFile.print(String(T));
    dataFile.print("\t"); //Tab 1 kali
    dataFile.print(String(v));
    dataFile.print("\t"); //Tab 1 kali
    dataFile.print(String(E));
    //menutup file setelah menggunakannya
    dataFile.close();
  }
  else {
    Serial.println("Gagal membuka file KAPUAS_3.XLS"); //Menampilkan kata "Gagal membuka  file NilaiPot.xls" diserial monitor laptop
  }
}

// https://randomnerdtutorials.com/esp32-microsd-card-arduino/
