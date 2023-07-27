// Multi UART bertujuan untuk dapat berkomunikasi secara UART ke beberapa device, 
// dalam kasus PCB Sensor adalah Sensor GPS dan Modbus PZEM-017 yang membutuhkan komunikasi UART
// Contoh di bawah sebagai komunikasi ESP32 sebagai Master dan 2 Mikrokontroler (Arduino Uno) sebagai Slave

#include <HardwareSerial.h>

String Pembacaan1, Pembacaan2;
char dataTerbaca1, dataTerbaca2;

HardwareSerial SerialPort1(1); // use UART1
HardwareSerial SerialPort2(2); // use UART2

void setup() {
  Serial.begin(115200);
  SerialPort1.begin(115200, SERIAL_8N1, 4, 2);    // RX1 TX1
  SerialPort2.begin(115200, SERIAL_8N1, 16, 17);  // RX2 TX2
}

void loop() {
  if (SerialPort1.available()) {
    dataTerbaca1 = SerialPort1.read();
    Pembacaan1 = Pembacaan1 + dataTerbaca1;
    if (dataTerbaca1 == '\n') {
      Serial.println("Data 1 = " + Pembacaan1);
      Pembacaan1 = "";
      delay(50);
    }
  }
  if (SerialPort2.available()) {
    dataTerbaca2 = SerialPort2.read();
    Pembacaan2 = Pembacaan2 + dataTerbaca2;
    if (dataTerbaca2 == '\n') {
      Serial.println("Data 2 = " + Pembacaan2);
      Pembacaan2 = "";
      delay(50);
    }
  }
}

// Muhammad Revaldi Frizky (27 Juli 2023)
