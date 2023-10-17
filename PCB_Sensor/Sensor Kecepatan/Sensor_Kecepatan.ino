#define Infra 25

unsigned long oldtime = 0;
unsigned long interval = 1000;
unsigned long timeold = 0; //Deklarasi nilai variabel oldtime = 0
int TIME; //Deklarasi Variabel time
float rev = 0; //Deklarasi nilai variabel rev = 0
float v = 0; //Kecepatan KM/Jam
float pi = 3.14;
float r = 0.03;
float rpm; //Deklarasi Variabel
float Speed = 0; //Deklarasi nilai variabel Speed = 0
float rotasi;

void isr()          //interrupt service routine
{
  rev++;
}

void setup() {
  pinMode(Infra, INPUT);
  Serial.begin(115200);
  attachInterrupt(Infra, isr, RISING);
}

void loop() {
  if ((millis() - oldtime) >= interval) {
    detachInterrupt(Infra);
    TIME = millis() - timeold;
    rpm = rev / TIME * 60000 ;
    timeold = millis();
    Speed = rpm * ((2.0 * pi * r) / 60.0);
    v = Speed * 3.6;
    oldtime = millis();
    Serial.println("Kecepatan rev = " + String(rev) + " putaran per detik");
    Serial.println("Kecepatan rpm = " + String(rpm) + " putaran per menit");
    Serial.println("Kecepatan speed = " + String(Speed) + " m/s");
    Serial.println("Kecepatan v = " + String(v) + " km/jam");
    Serial.println("===========================");
    Serial.println(" ");
    attachInterrupt(Infra, isr, RISING);
    rev = 0;
  }
}
