#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

#define DHTPIN 4       // Pin data DHT22 terhubung ke D4
#define DHTTYPE DHT22
#define LED_BIRU 2     // LED biru bawaan board ESP32

const char* ssid        = "paramadina_cikarang";
const char* password    = "WIFI_PASSWORD";
const char* mqtt_server = "192.168.10.86";
const int   mqtt_port   = 1883;
const char* mqtt_user   = "esp32";
const char* mqtt_pass   = "MQTT_PASSWORD";
const char* mqtt_topic  = "iot/kel1/dht22";

DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BIRU, !digitalRead(LED_BIRU)); // Kedip-kedip saat mencari WiFi
    delay(300);
  }
  digitalWrite(LED_BIRU, HIGH); // Menyala stabil saat WiFi terhubung
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32_Kel1_Client", mqtt_user, mqtt_pass)) {
      digitalWrite(LED_BIRU, HIGH); // Menyala stabil saat MQTT terhubung
    } else {
      digitalWrite(LED_BIRU, LOW);  // Mati jika gagal konek MQTT
      delay(3000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_BIRU, OUTPUT);
  digitalWrite(LED_BIRU, LOW); // Mati saat belum konek
  dht.begin();

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    setup_wifi();
  }
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  float suhu = dht.readTemperature();
  float kelembapan = dht.readHumidity();

  if (!isnan(suhu) && !isnan(kelembapan)) {
    String payload = "{\"suhu\":" + String(suhu, 2) + ",\"kelembapan\":" + String(kelembapan, 2) + "}";
    Serial.println("Kirim: " + payload);
    client.publish(mqtt_topic, payload.c_str());
  }

  delay(10000); // Kirim data setiap 10 detik
}
