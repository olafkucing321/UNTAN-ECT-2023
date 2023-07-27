#define pinPot 34
float nilaiTeg;
float nilaiTeg2;

void setup() {
  Serial.begin(115200);
  pinMode(pinPot,INPUT);
  Serial.println("Siap");
}

void loop() {
  int nilaiADC = analogRead(pinPot);
  Serial.println(nilaiADC);
  int nilaiMap = map(nilaiADC, 3206, 4095, 263, 306);
  int nilaisesungguhnya = map(nilaiMap, 263, 306, 434, 504);
  nilaiTeg = nilaiMap/100.0;
  nilaiTeg2 = nilaisesungguhnya/10.0;
  Serial.println("Tegangan masuk: " + String(nilaiTeg2));
  delay(50);
}
