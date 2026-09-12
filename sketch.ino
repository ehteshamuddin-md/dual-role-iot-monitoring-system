#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// Wi-Fi
const char* WIFI_SSID = "Wokwi-GUEST";
const char* WIFI_PASSWORD = "";

// MQTT
const char* MQTT_SERVER = "broker.hivemq.com";
const int MQTT_PORT = 1883;

// Pins
#define HEART_RATE_PIN 34
#define SPO2_PIN 35
#define BODY_TEMP_PIN 32
#define OXYGEN_PIN 33
#define AQI_PIN 36
#define DHT_PIN 4
#define DHT_TYPE DHT22

DHT dht(DHT_PIN, DHT_TYPE);
WiFiClient espClient;
PubSubClient mqttClient(espClient);

// MQTT Topics
const char* TOPIC_HEART_RATE =
    "hospital/patient/heart_rate";

const char* TOPIC_SPO2 =
    "hospital/patient/spo2";

const char* TOPIC_BODY_TEMP =
    "hospital/patient/body_temperature";

const char* TOPIC_ROOM_TEMP =
    "hospital/facility/room_temperature";

const char* TOPIC_OXYGEN =
    "hospital/facility/oxygen_level";

const char* TOPIC_AQI =
    "hospital/facility/aqi";

// Wi-Fi connection
void connectWiFi() {

  Serial.print("Connecting to WiFi");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected!");

  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());
}

// MQTT connection
void connectMQTT() {

  while (!mqttClient.connected()) {

    Serial.print("Connecting to MQTT...");

    String clientID = "WokwiHospitalESP32-";
    clientID += String(random(0xffff), HEX);

    if (mqttClient.connect(clientID.c_str())) {

      Serial.println("connected!");

    } else {

      Serial.print("failed, state=");
      Serial.println(mqttClient.state());

      delay(2000);
    }
  }
}

// Setup
void setup() {

  Serial.begin(115200);

  pinMode(HEART_RATE_PIN, INPUT);
  pinMode(SPO2_PIN, INPUT);
  pinMode(BODY_TEMP_PIN, INPUT);
  pinMode(OXYGEN_PIN, INPUT);
  pinMode(AQI_PIN, INPUT);

  dht.begin();

  connectWiFi();

  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);

  connectMQTT();

  Serial.println();
  Serial.println("=================================");
  Serial.println(" ESP32 Hospital IoT Simulator");
  Serial.println("=================================");
}

// Main loop
void loop() {

  if (!mqttClient.connected()) {
    connectMQTT();
  }

  mqttClient.loop();

  // Read potentiometers
  int heartRaw = analogRead(HEART_RATE_PIN);
  int spo2Raw = analogRead(SPO2_PIN);
  int bodyTempRaw = analogRead(BODY_TEMP_PIN);
  int oxygenRaw = analogRead(OXYGEN_PIN);
  int aqiRaw = analogRead(AQI_PIN);

  // Convert readings
  int heartRate =
      map(heartRaw, 0, 4095, 50, 110);

  int spo2 =
      map(spo2Raw, 0, 4095, 90, 100);

  float bodyTemperature =
      35.0 + ((float)bodyTempRaw / 4095.0) * 4.0;

  float oxygenLevel =
      18.0 + ((float)oxygenRaw / 4095.0) * 7.0;

  int aqi =
      map(aqiRaw, 0, 4095, 0, 100);

  // Read room temperature from DHT22
  float roomTemperature = dht.readTemperature();

  if (isnan(roomTemperature)) {

    Serial.println("DHT22 reading failed!");

    return;
  }

  // Convert values to strings
  char heartRateStr[10];
  char spo2Str[10];
  char bodyTempStr[10];
  char roomTempStr[10];
  char oxygenStr[10];
  char aqiStr[10];

  sprintf(heartRateStr, "%d", heartRate);
  sprintf(spo2Str, "%d", spo2);

  dtostrf(bodyTemperature, 4, 1, bodyTempStr);
  dtostrf(roomTemperature, 4, 1, roomTempStr);
  dtostrf(oxygenLevel, 4, 1, oxygenStr);

  sprintf(aqiStr, "%d", aqi);

  // Publish patient data
  mqttClient.publish(
      TOPIC_HEART_RATE,
      heartRateStr);

  mqttClient.publish(
      TOPIC_SPO2,
      spo2Str);

  mqttClient.publish(
      TOPIC_BODY_TEMP,
      bodyTempStr);

  // Publish facility data
  mqttClient.publish(
      TOPIC_ROOM_TEMP,
      roomTempStr);

  mqttClient.publish(
      TOPIC_OXYGEN,
      oxygenStr);

  mqttClient.publish(
      TOPIC_AQI,
      aqiStr);

  // Serial Monitor
  Serial.println();

  Serial.println("----- PATIENT DATA -----");

  Serial.print("Heart Rate       : ");
  Serial.print(heartRate);
  Serial.println(" BPM");

  Serial.print("SpO2             : ");
  Serial.print(spo2);
  Serial.println(" %");

  Serial.print("Body Temperature : ");
  Serial.print(bodyTemperature, 1);
  Serial.println(" °C");

  Serial.println();

  Serial.println("----- FACILITY DATA -----");

  Serial.print("Room Temperature : ");
  Serial.print(roomTemperature, 1);
  Serial.println(" °C");

  Serial.print("Oxygen Level     : ");
  Serial.print(oxygenLevel, 1);
  Serial.println(" %");

  Serial.print("AQI              : ");
  Serial.println(aqi);

  Serial.println("-------------------------");

  delay(5000);
}
