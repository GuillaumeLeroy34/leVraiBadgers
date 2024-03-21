#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN D8  //
#define RST_PIN D0 //

#define buzzer D1
#define rouge D2
#define vert  D3

MFRC522 mfrc522(SS_PIN, RST_PIN);
// Create MFRC522 instance.
int statuss = 0;
int out = 0;
int compterHampter = 0;

const char *ssid = "imagine";
const char *password = "12345678";
//? wifi

const char *mqttserver = "maqiatto.com";
const int mqttport = 1883;
const char *user = "fx4431@gmail.com";
const char *passwordMq = "badger";

WiFiClient espClient;
PubSubClient client(espClient);

void printHex(byte *buffer, byte bufferSize)
{
  for (byte i = 0; i < bufferSize; i++)
  {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

void printDec(byte *buffer, byte bufferSize)
{
  for (byte i = 0; i < bufferSize; i++)
  {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}

void callback(char *topic, byte *payload, int length)
{

  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  Serial.print("Message:");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.println("-----------------------");

  if ((char)payload[0] == '0')
  {
    Serial.println("0 detecté, opinion refusé");
    pinMode(buzzer,HIGH);
    pinMode(rouge,HIGH);
  }
  else if ((char)payload[0] == '1')
  {
    Serial.println("1 detecté, opinion validé");
    pinMode(vert,HIGH);
  }
}

void setup()
{

  Serial.begin(115200);              // Initiate a serial communication
  SPI.begin();                       // Initiate  SPI bus
  mfrc522.PCD_Init();                // Initiate MFRC522
  mfrc522.PCD_DumpVersionToSerial(); // Affichage des données de la bibliothèque
  Serial.println(F("Scan PICC to see UID, type, and data blocks..."));

  // connexion au wifi, et au broker mqtt
  {
    Serial.begin(115200);
    delay(1000);
    Serial.println();
    client.setServer(mqttserver, mqttport);
    client.setCallback(callback);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
    }

    Serial.println();

    Serial.println("Wi-Fi Connecté Succès !");
    Serial.print("NodeMCU IP Address : ");
    Serial.println(WiFi.localIP());

    while (!client.connected())
    {
      Serial.println("Connecting to MQTT...");

      if (client.connect("mqttIOT", user, passwordMq))
      {

        Serial.println("connected");
      }
      else
      {

        Serial.print("failed with state ");
        Serial.print(client.state());
        delay(2000);
      }
      client.subscribe("fx4431@gmail.com/reponse");
    }

    delay(200);
  }
}

void loop()
{

  client.loop();
  if (!mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Récupération des informations de la carte RFID
  if (!mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  // Affichage des informations de la carte RFID
  // Serial.println(F("The NUID tag is:"));
  // Serial.print(F("In hex: "));
  // printHex(mfrc522.uid.uidByte, mfrc522.uid.size);
  // Serial.println();
  // Serial.print(F("In dec: "));
  // printDec(mfrc522.uid.uidByte, mfrc522.uid.size);

  char *ms = (char *)mfrc522.uid.uidByte;

  client.publish("fx4431@gmail.com/badger", ms);
  Serial.println();
}
