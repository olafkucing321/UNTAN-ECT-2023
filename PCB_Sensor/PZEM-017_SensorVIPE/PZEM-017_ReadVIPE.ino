// Sebelum jalankan program ini, jalankan program set shunt & set address untuk PZEM-017 baru

#include <ModbusMasterPzem017.h>
static uint8_t pzemSlaveAddr = 0x01;                                                        //  Alamat PZEM

float PZEMVoltage, PZEMCurrent, PZEMPower, PZEMEnergy = 0;
bool ResetEnergy;

ModbusMaster PZEM;

void setup(){
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N2);                                                           // Untuk PZEM, gunakan Baud rate 9600, 8 data bits, 2 stop bit, no parity
  PZEM.begin(pzemSlaveAddr, Serial2);
  delay(1000);
}

void loop(){
  ////////////////////////////////////////////  RESET ENERGI MELALUI SERIAL MONITOR    /////////////////////////////////////////////////////////////////////
  if (Serial.available() > 0) {
    int Pembacaan = Serial.read();
    if (Pembacaan == 'R') {
      ResetEnergy = true;
      Serial.println("Reset Energi");
    }
    else {
      Serial.print("");
    }
  }
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  PZEM_DC_Meter();
  delay(5000);
}

void PZEM_DC_Meter() {
  if (ResetEnergy == true) {
    uint16_t u16CRC = 0xFFFF;                                                               // Deklarasi CRC check 16 bits
    static uint8_t resetCommand = 0x42;                                                     // Baca Data Sheet Bagian 2.5 Reset energy
    uint8_t slaveAddr = 0X01;
    u16CRC = crc16_update(u16CRC, slaveAddr);
    u16CRC = crc16_update(u16CRC, resetCommand);
    
    Serial2.write(slaveAddr); Serial2.write(resetCommand); Serial2.write(lowByte(u16CRC)); Serial2.write(highByte(u16CRC));
    delay(200);
    ResetEnergy = false;
  }
  
  uint8_t result;                                                                           // Baca Data Sheet Bagian 2.3 Read the measurement result
  result = PZEM.readInputRegisters(0x0000, 6); 
  if (result == PZEM.ku8MBSuccess) {
    uint32_t tempdouble = 0x00000000;
    PZEMVoltage = PZEM.getResponseBuffer(0x0000) / 100.0;                                   // 0x0000 lihat di tabel -> Voltage Value (Voltage 10000,display 100.00V)
    Serial.print(PZEMVoltage, 2); Serial.print("V   ");

    PZEMCurrent = PZEM.getResponseBuffer(0x0001) / 100.0;                                   // 0x0001 lihat di tabel -> Current Value (Current 100,display 1.000A)
    Serial.print(PZEMCurrent, 3); Serial.print("A   ");

    tempdouble =  (PZEM.getResponseBuffer(0x0003) << 16) + PZEM.getResponseBuffer(0x0002);  // Daya menggunakan data 32 bit, sehingga perlu 2 Buffer 16 bit
    PZEMPower = tempdouble / 10.0;                                                          // 0x0002 & 0x0003 -> Power Value (Power 1000,display 100.0W)
    Serial.print(PZEMPower, 1); Serial.print("W  ");

    tempdouble =  (PZEM.getResponseBuffer(0x0005) << 16) + PZEM.getResponseBuffer(0x0004);  // Energi menggunakan data 32 bit, sehingga perlu 2 Buffer 16 bit
    PZEMEnergy = tempdouble;                                                                // 0x0004 & 0x0005 -> Energy Value (Energy 0,display 0Wh)
    Serial.print(PZEMEnergy, 0); Serial.print("Wh  ");

    Serial.println();
  } else {
    Serial.println("Gagal mendapatkan data PZEM-017");
  }
}

// Muhammad Revaldi Frizky (9 Agustus 2023)
// Sumber: https://www.youtube.com/watch?v=eLuW8vf4qDM&pp=ygUNcHplbTAxNyBlc3AzMg%3D%3D
