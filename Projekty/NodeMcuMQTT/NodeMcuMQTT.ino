#include <DHT.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#define DHTTYPE DHT22
uint8_t DHTPIN = D5;

#define wifi_ssid "Domek_33_3G"
#define wifi_password "281128MO"

#define mqtt_server "192.168.1.3"
#define mqtt_user "Sensor"
#define mqtt_password ""

#define humidity_topic "Home/Sensor/Humidity"
#define temperature_topic "Home/Sensor/Temperature"
#define state_topic "Home/Sensor/State"
float Temperatura;
float Wilgotnosc;

WiFiClient espClient;
void callback(char* topic, byte* payload, unsigned int length);
PubSubClient client(mqtt_server, 1883, callback, espClient);
DHT dht(DHTPIN, DHTTYPE, 11); // 11 works fine for ESP8266

void setup() {
  pinMode(DHTPIN, INPUT);
  pinMode(D7, OUTPUT);
  Serial.begin(115200);
  dht.begin();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  client.subscribe("Home\Sensor\Command");
}
void callback(char* topic, byte* payload, unsigned int length) {
  client.publish(state_topic, "ON", 1);
  digitalWrite(D7, LOW);
}
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    if (client.connect("ESP8266Client")) {
      //if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

long lastMsg = 0;
float temp = 0.0;
float hum = 0.0;
void loop() {
  
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  delay(10);
  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;

    temp = dht.readTemperature();
    hum = dht.readHumidity();

    //if (checkBound(newTemp, temp, diff)) {
    //temp = newTemp;
    //Serial.print("New temperature:");
    //Serial.println(String(temp).c_str());
    //client.publish(temperature_topic, String(temp).c_str(), true);
    //}

    //if (checkBound(newHum, hum, diff)) {
    //hum = newHum;
    //Serial.print("New humidity:");
    //Serial.println(String(hum).c_str());
    client.publish(humidity_topic, String(hum).c_str(), true);
    //}
  }
}
