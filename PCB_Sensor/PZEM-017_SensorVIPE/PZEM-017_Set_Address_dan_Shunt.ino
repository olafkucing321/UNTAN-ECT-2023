#include <ModbusMasterPzem017.h>
static uint8_t pzemOldAddr = 0xF8;  //Broadcast Address - Kalau ndak bisa pakai 0x00
static uint8_t pzemNewAddr = 0x01;  //New Address
static uint16_t NewshuntAddr = 0x0001;                                                      //  "0x0001" untuk tahanan shunt 50A
ModbusMaster node;

void setup()
{
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N2);
  changeAddress(pzemOldAddr, pzemNewAddr);
  delay(200);
  setShunt(pzemNewAddr);
  delay(200);
  node.begin(pzemNewAddr, Serial2);
  delay(1000);
} //Setup Ends

void loop() {
  uint8_t result;
  result = node.readInputRegisters(0x0000, 6);
  if (result == node.ku8MBSuccess) {
    Serial.print("Volts:   "); Serial.println(node.getResponseBuffer(0x0000) / 100.0, 1); //Print Voltage value on Serial Monitor with 1 decimal
  } else {
    Serial.println("Failed to read modbus");
  }
  delay(5000);
} //Loop Ends

void changeAddress(uint8_t OldslaveAddr, uint8_t NewslaveAddr) {
  static uint8_t SlaveParameter = 0x06; //Write command code to PZEM
  static uint16_t registerAddress = 0x0002; // Modbus RTU device address command code

  uint16_t u16CRC = 0xFFFF; //declare CRC check 16 bits
  u16CRC = crc16_update(u16CRC, OldslaveAddr);  // Calculate the crc16 over the 6bytes to be send
  u16CRC = crc16_update(u16CRC, SlaveParameter);
  u16CRC = crc16_update(u16CRC, highByte(registerAddress));
  u16CRC = crc16_update(u16CRC, lowByte(registerAddress));
  u16CRC = crc16_update(u16CRC, highByte(NewslaveAddr));
  u16CRC = crc16_update(u16CRC, lowByte(NewslaveAddr));

  Serial.println("Change Slave Address");
  Serial2.write(OldslaveAddr); //these whole process code sequence refer to manual
  Serial2.write(SlaveParameter);
  Serial2.write(highByte(registerAddress));
  Serial2.write(lowByte(registerAddress));
  Serial2.write(highByte(NewslaveAddr));
  Serial2.write(lowByte(NewslaveAddr));
  Serial2.write(lowByte(u16CRC));
  Serial2.write(highByte(u16CRC));
  delay(10); delay(100);
  while (Serial2.available()) {
    Serial.print(char(Serial2.read()), HEX); //Prints the response and display on Serial Monitor (Serial)
    Serial.print(" ");
  }
}

void setShunt(uint8_t slaveAddr) {
  static uint8_t SlaveParameter = 0x06;   // 0x06 command Write Single Register             // Baca Data Sheet Bagian 2.4 Read and modify the slave parameters
  static uint16_t registerAddress = 0x0003;                                                 // 0x0003 lihat di tabel -> The current range

  uint16_t u16CRC = 0xFFFF;
  u16CRC = crc16_update(u16CRC, slaveAddr);
  u16CRC = crc16_update(u16CRC, SlaveParameter);
  u16CRC = crc16_update(u16CRC, highByte(registerAddress));
  u16CRC = crc16_update(u16CRC, lowByte(registerAddress));
  u16CRC = crc16_update(u16CRC, highByte(NewshuntAddr));
  u16CRC = crc16_update(u16CRC, lowByte(NewshuntAddr));

  Serial2.write(slaveAddr); Serial2.write(SlaveParameter); Serial2.write(highByte(registerAddress)); Serial2.write(lowByte(registerAddress)); 
  Serial2.write(highByte(NewshuntAddr)); Serial2.write(lowByte(NewshuntAddr)); Serial2.write(lowByte(u16CRC)); Serial2.write(highByte(u16CRC));
  delay(200);
  while (Serial2.available()) {
    Serial.print(char(Serial2.read()), HEX);
    Serial.print(" ");
  }
}
