//Gracias a un lector de tarjetas RFID segun el codigo de la tarjeta dejara pasar o no y nos mandara mensajes por telegram y si el codigo no esta registrado sacara un foto que nos mandara a telegram todo esto a traves de node red 

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <MFRC522.h>



const char* ssid = "AULA17";
const char* password = "123456789";
const char* mqtt_server = "192.168.17.22";


#define RST_PIN D3
#define SS_PIN D4

MFRC522 reader(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

byte validKey1[4] = { 0xF2, 0xD2, 0x5B, 0x1A };
byte validKey2[4] = { 0x93, 0x6B, 0xA6, 0x1A };

bool isEqualArray(byte* arrayA, byte* arrayB, int length)
{
  for (int index = 0; index < length; index++)
  {
    if (arrayA[index] != arrayB[index]) return false;
  }
  return true;
}

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];


void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}



void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("prueba", "ruben");
      // ... and resubscribe
      
     
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
 
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  SPI.begin();        
  reader.PCD_Init();
  
 
}

void loop() {

if (!client.connected()) {
    reconnect();
  }
   client.loop();
 unsigned long now = millis();
   if (reader.PICC_IsNewCardPresent()) 
  {
    //Seleccionamos una tarjeta
    if (reader.PICC_ReadCardSerial())
    {
      // Comparar ID con las claves válidas
      if (isEqualArray(reader.uid.uidByte, validKey1, 4) || isEqualArray(reader.uid.uidByte, validKey2, 4)){
        Serial.println("Tarjeta valida");
      }else{
        Serial.println("Tarjeta invalida");
        client.publish("tarjeta/denegado", "Estan intentando entrar sin permiso");
    }
     if (isEqualArray(reader.uid.uidByte, validKey1, 4)){
        Serial.println("Es pepin loquete");
        client.publish("tarjeta/pepin", "Ha entrado Pepin");
     }
     if (isEqualArray(reader.uid.uidByte, validKey2, 4)){
        Serial.println("Es joseba loquete"); 
        client.publish("tarjeta/joseba", "Ha entado Joseba");
     }

      // Finalizar lectura actual
      reader.PICC_HaltA();
    }
  }
  delay(250);

  
  }