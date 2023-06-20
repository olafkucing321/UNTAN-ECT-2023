#define Arus 35

#define nilaiKoreksi 164 //mA
#define iterasi 10000
#define teganganReferensi 3300.0 //5000mv is for 5V
#define bitResolusi 12

float I = 0;
unsigned long oldtime, oldtime2 = 0;
unsigned long interval = 1000;

void setup() {  
  Serial.begin(115200);   //Serial komunikasi untuk dihubungkan ke laptop/PC dengan baud rate 115200
  delay(1000);
}

void loop() {
  if((millis() - oldtime) >= interval){
    Sensor_Arus();
    oldtime = millis();
  }
}

void Sensor_Arus(){
  int nilaiSensor;
  float voltage, current;
  if((millis() - oldtime2) >= interval){
    for(int i=0; i<= iterasi; i++)
      {   
        nilaiSensor = analogRead(Arus);
          voltage = (nilaiSensor) * (teganganReferensi / (pow(2,bitResolusi)-1)) - ( 0.5 * teganganReferensi) + nilaiKoreksi ; 
          current  = voltage / 66.0  ;
          I += current;
    }
    oldtime2 = millis();
  }
  I /= iterasi;
   I -= 2.59;
//    if(I < 0){
//      I = 0;
//    }
    Serial.println("Arus : " + String(I));
    Serial.println("nilaiSensor : " + String(nilaiSensor));
}
