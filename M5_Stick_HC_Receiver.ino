#include <M5StickCPlus.h>
#include <WiFi.h>
#include <PubSubClient.h>

// WiFi credentials
// Define WiFi credentials
const char* ssid = "utexas-iot"; // WiFi Network Name
const char* password = "11074388945493829140"; // Password

// MQTT Broker details
const char* mqtt_broker = "34.27.189.200";
const char* mqtt_username = "msquser";
const char* mqtt_password = "msqpass";
const int mqtt_port = 1883;
const char* mqtt_topic = "topicalert";

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  // Connect to WiFi
  M5.Lcd.println("Connecting to WiFi..");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    M5.Lcd.print(".");
  }
  M5.Lcd.println("WiFi Connected!");
}

void callback(char* topic, byte* payload, unsigned int length) {
  M5.Lcd.print("Message arrived [");
  M5.Lcd.print(topic);
  M5.Lcd.print("] ");
  for (int i = 0; i < length; i++) {
    M5.Lcd.print((char)payload[i]);
  }
  M5.Lcd.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    M5.Lcd.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("M5StickCPlusClient", mqtt_username, mqtt_password)) {
      M5.Lcd.println("connected");
      // Subscribe
      client.subscribe(mqtt_topic);
    } else {
      M5.Lcd.print("failed, rc=");
      M5.Lcd.print(client.state());
      M5.Lcd.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 10);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(WHITE);

  setup_wifi();
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  M5.update(); // Add this to update button status
}
