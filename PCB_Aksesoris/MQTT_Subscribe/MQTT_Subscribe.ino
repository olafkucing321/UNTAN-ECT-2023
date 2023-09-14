#include <WiFi.h>
#include <PubSubClient.h>

const char *ssid = "NAMA_WIFI";
const char *password = "SANDI_WIFI";

// MQTT Broker
const char *mqtt_broker = "broker.emqx.io";
const char *topic = "emqx/esp32/untan-ect";
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const int mqtt_port = 1883;

String PesanData;
int V, I, P, T, v, E, W;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("Menghubungkan ke WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Terhubung");

  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  while (!client.connected()) {
    String client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("EMQX MQTT broker publik terhubung");
    } else {
      Serial.print("Gagal dengan status ");
      Serial.print(client.state());
      delay(2000);
    }
  }
  client.subscribe(topic);
}

void callback(char *topic, byte *payload, unsigned int length) {
  for (int i = 0; i < length; i++) {
    PesanData = PesanData + String((char)payload[i]);
  }
  int JumlahData = 7;           // Jumlah daya yang dikirim
  String Data[JumlahData];
  for (int i = 0; i < JumlahData; i++) {
    Data[i] = getValue(PesanData, ',', i);
  }
  PesanData = "";

  V = Data[0].toInt();
  I = Data[1].toInt();
  P = Data[2].toInt();
  T = Data[3].toInt();
  v = Data[4].toInt();
  W = Data[5].toInt();
  E = Data[6].toInt();

  Serial.println("V = " + String(V) + ", I = " + String(I) + ", P = " + String(P) + ", T = " + String(T) + ", v = " + String(v) + ", W = " + String(W) + ", E = " + String(E));
}

void loop() {
  if (!client.connected()) {
    String client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("EMQX MQTT broker publik terhubung");
    } else {
      Serial.print("Gagal dengan status ");
      Serial.println(client.state());
      delay(2000);
    }
  }
  client.loop();
}

String getValue(String data, char separator, int index) {
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

// Muhammad Revaldi Frizky - 9 Juli 2023
// Sumber: https://www.emqx.com/en/blog/esp32-connects-to-the-free-public-mqtt-broker
