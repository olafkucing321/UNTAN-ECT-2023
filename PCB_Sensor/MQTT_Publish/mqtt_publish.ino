#include <PubSubClient.h>
#include <WiFi.h>

int A, B, C, D;
String Kirim_data;

// WiFi
const char *ssid = "IpXr"; // Enter your Wi-Fi name
const char *password = "assalamualaikum";  // Enter Wi-Fi password

// MQTT Broker
const char *mqtt_broker = "broker.emqx.io";
const char *topic = "emqx/esp32/qodar";
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  // Set software serial baud to 115200;
  Serial.begin(115200);
  // Connecting to a WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the Wi-Fi network");
  //connecting to a mqtt broker
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  while (!client.connected()) {
    String client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());
    Serial.printf("The client %s connects to the public MQTT broker\n", client_id.c_str());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Public EMQX MQTT broker connected");
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char) payload[i]);
  }
  Serial.println();
  Serial.println("-----------------------");
}

void loop() {
  client.loop();
  for (;;) {
    A = random (0, 10);
    B = random (0, 10) * 10;

    if (A == 0) {
      Serial.println("Nilai A adalah nol");
      continue;
    }
    C = B / A;
    D = B % A;

    Kirim_data = String (A) + "," + String(B) + "," + String(C) + "," + String(D);
    client.publish(topic, Kirim_data.c_str());
    Serial.print("A = ");
    Serial.println(A);
    Serial.print("B = ");
    Serial.println(B);
    Serial.print("C = ");
    Serial.println(C);
    Serial.print("D = ");
    Serial.println(D);
    delay(10000);
    void callback();
  }
}
