#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// put function declarations here:
const char* ssid = "imagine";
const char* password = "12345678";
//? wifi

const char* mqttserver = "maqiatto.com";
const int mqttport = 1883;
const char* user = "fx4431@gmail.com";
const char* passwordMq = "badger";

WiFiClient espClient;
PubSubClient client(espClient);


void callback(char* topic, byte* payload, int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.println("-----------------------");

}

void setup() {

  Serial.begin(115200);
  delay(1000);
  Serial.println();
  client.setServer(mqttserver, mqttport);
  client.setCallback(callback);
  WiFi.begin(ssid,password);



  while( WiFi.status() != WL_CONNECTED ){
      delay(500);
      Serial.print(".");
  }

  Serial.println();

  Serial.println("Wi-Fi Connecté Succès !");
  Serial.print("NodeMCU IP Address : ");
  Serial.println(WiFi.localIP() );

while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("mqttIOT", user, passwordMq )) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
     client.subscribe("fx4431@gmail.com/badger");
  }
    client.publish("fx4431@gmail.com/badger", "get snaked"); //Topic name
  
  delay(200);


}

void loop() {
  // put your main code here, to run repeatedly:
  client.loop();
}
