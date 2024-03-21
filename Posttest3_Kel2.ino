#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <DHT.h>
#include <ThingSpeak.h>

#define LED_GREEN D2
#define LED_YELLOW D1
#define LED_RED D0
#define BUZZER D8
#define DHTPIN D4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const char *ssid = "Universitas Mulawarman"; 
const char *password = ""; 
WiFiClient client; 

unsigned long channel = 2477406; 
const char *myWriteAPIKey = "39P9J9MM7D2SCYPG";
const char *myReadAPIKey = "HV763VT8NSR252K3";
uint8_t temperature, humidity;
unsigned int suhu1 = 1;
unsigned int kelembaban1 = 2;
unsigned int led1 = 3;
unsigned int feedback1 = 4;

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_RED, LOW);
  ThingSpeak.begin(client);
  
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
  ThingSpeak.begin(client);
}

void loop() {
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  Serial.print("Temperature Value is :");
  Serial.print(temperature);
  Serial.println("C");

  Serial.print("Humidity Value is :");
  Serial.print(humidity);
  Serial.println("%");
  
  ThingSpeak.writeField(channel, 1, temperature, myWriteAPIKey); 
  ThingSpeak.writeField(channel, 2, humidity, myWriteAPIKey); 
  
  if(temperature > 36) {
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_RED, HIGH);
    tone(BUZZER, 1000); // Peringatan suara
    // delay(1000); // Delay untuk peringatan suara
    // noTone(BUZZER); // Mematikan suara
    ThingSpeak.writeField(channel, feedback1, "WARNING: Temperature above 36°C!", myWriteAPIKey);
  } else if (temperature >= 30 && temperature <= 36) {
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, HIGH);
    digitalWrite(LED_RED, LOW);
    noTone(BUZZER); // Mematikan suara
    ThingSpeak.writeField(channel, feedback1, "Temperature within normal range", myWriteAPIKey);
  } else  {
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_RED, LOW);
    noTone(BUZZER); // Mematikan suara
    ThingSpeak.writeField(channel, feedback1, "Temperature within normal range", myWriteAPIKey);
  }
  
  delay(1000);
}
