#include <ESP8266WiFi.h>
#include <SPI.h>
#include "MFRC522.h"
#include <ArduinoJson.h>
#include <Constellation.h>
#include <String.h>
#include <Servo.h>

Servo monServo; 

/* wiring the MFRC522 to ESP8266 (ESP-12)
RST     = D1
SDA(SS) = D2
MOSI    = D7
MISO    = D6
SCK     = D5
GND     = GND
3.3V    = 3.3V
Echo    = D3
Trig    = D4
*/

#define RST_PIN  5  // RST-PIN für RC522 - RFID - SPI - Modul GPIO5 
#define SS_PIN  4  // SDA-PIN für RC522 - RFID - SPI - Modul GPIO4 

char ssid[]="Lemien";
char mdp[] = "prtz6908";

/* Constantes pour le timeout */
const unsigned long MEASURE_TIMEOUT = 25000UL; // 25ms = ~8m à 340m/s

/* Vitesse du son dans l'air en mm/us */
const int TRIGGER = 2; //D4
const int ECHO = 0; //D3

int defaultValue = -1;

Constellation<WiFiClient> constellation("192.168.43.11", 8088, "ESP8266", "ESP8266_Package", "AZERTY");

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

void dump_byte_array(byte *buffero, byte bufferSize) {
  delay(50);
  String str[bufferSize];
  String so;
 delay(50);
  for (byte i = 0; i < bufferSize; i++) {
    str[i]= String(buffero[i], HEX);
    delay(100); //100
    so += str[i];
    delay(100); //100
  }
  delay(100); //100
  Serial.print(so);
  delay(100); //100
  char sochar[256];
  delay(100); //100
  so.toCharArray(sochar, 256);
  delay(100); //100
  constellation.pushStateObject("badge", sochar, 10);
  delay(500); //500
}

void NFC() { 
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  // Show some details of the PICC (that is: the tag/card)
  Serial.print(F("Card UID:"));
  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  Serial.println();
}

void setup(){
  pinMode(TRIGGER, OUTPUT);
  digitalWrite(TRIGGER, LOW); // La broche TRIGGER doit être à LOW au repos
  pinMode(ECHO, INPUT);
  Serial.begin(9600);
  monServo.attach(15); 

  // Connect to Wifi  
  WiFi.begin(ssid, mdp);  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // For WiFi101, wait 3 seconds for connection!
  delay(3000);
 
  Serial.println("WiFi connected. IP: ");
  Serial.println(WiFi.localIP());
  // Declare the package descriptor
  //constellation.writeInfo("hello world on inter galactical project"); // essai
  constellation.declarePackageDescriptor();
    delay(50);
  Serial.println(F("Booting...."));
  
  SPI.begin();           // Init SPI bus
  Serial.println("Scan PICC to see UID and type...");
  mfrc522.PCD_Init();    // Init MFRC522
  
  Serial.println(F("Ready!"));
  Serial.println(F("======================================================")); 
  Serial.println(F("Scan for Card and print UID:"));
  NFC();
  monServo.write(160);
  
  constellation.registerStateObjectLink("*", "brain", "ActivateMotor", [](JsonObject& so) {
    delay(100); // délai afin d'attendre la connexion à Constellation
    if (so["Value"]== true){   
      monServo.write(160);  // Position de la porte ouverte
      }
    if (so["Value"]== false){
      monServo.write(55);   // Position de la porte fermée
      }
  });  
}

// Puch on Constellation distance
void pushOnConstellation(int dist){
      constellation.pushStateObject("distance", stringFormat("{'distance':%d, 'defaultValue':%d}", dist, defaultValue), 10);
}



// Helper routine to dump a byte array as hex values to Constelation


void loop(){
  // Process incoming message & StateObject updates
  //digitalWrite(pin, HIGH);
  
  /* 1. Lance une mesure de distance en envoyant une impulsion HIGH de 10µs sur la broche TRIGGER */
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER, LOW);
  
  /* 2. Mesure le temps entre l'envoi de l'impulsion ultrasonique et son écho (si il existe) */
  long measure = pulseIn(ECHO, HIGH);
   
  /* 3. Calcul la distance à partir du temps mesuré */
  long distanceMeasured = measure / 58.2;
  if(defaultValue == -1){
    defaultValue = 10;
        Serial.print("default value : ");
        Serial.println(defaultValue);

  }
  if(distanceMeasured < defaultValue){
    /* 4.Affiche les résultats en mm, cm et m */
    Serial.print(" Distance: ");
    Serial.print(distanceMeasured);
    Serial.print(" cm ");
    Serial.println("");
    delay(50);
    //pushOnConstellation(distanceMeasured);
    constellation.sendMessage(Package, "brain", "Message", "[ 'Demo from Arduino', 'Ceci est une demo', 'Device', '' ]");
  }
  
  /* Délai d'attente pour éviter d'afficher trop de résultats à la seconde */

  constellation.loop();
  NFC();
  }
