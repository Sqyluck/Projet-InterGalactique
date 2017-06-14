#include <ESP8266WiFi.h>
#include <SPI.h>
#include "MFRC522.h"
#include <ArduinoJson.h>
#include <Constellation.h>
#include <String.h>


/* wiring the MFRC522 to ESP8266 (ESP-12)
RST     = D1
SDA(SS) = D2
MOSI    = D7
MISO    = D6
SCK     = D5
GND     = GND
3.3V    = 3.3V
*/

#define RST_PIN  5  // RST-PIN für RC522 - RFID - SPI - Modul GPIO5 
#define SS_PIN  4  // SDA-PIN für RC522 - RFID - SPI - Modul GPIO4 

const char *ssid =  "Xperia Z3_b000";     // change according to your Network - cannot be longer than 32 characters!
const char *mdp =  "63a5cd0d0472"; // change according to your Network

Constellation<WiFiClient> constellation("192.168.43.55", 8088, "MyVirtualSentinel", "MyVirtualPackage", "c6f23fb2987df8d36f00d10a370a0b85ee7d7a26");

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

void setup() {
  Serial.begin(9600);    // Initialize serial communications

    WiFi.begin(ssid, mdp);  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // For WiFi101, wait 10 seconds for connection!
  delay(3000);
 
  Serial.println("WiFi connected. IP: ");
  Serial.println(WiFi.localIP());
  
  delay(50);
  Serial.println(F("Booting...."));
  
  SPI.begin();           // Init SPI bus
  Serial.println("Scan PICC to see UID and type...");
  mfrc522.PCD_Init();    // Init MFRC522
  
  Serial.println(F("Ready!"));
  Serial.println(F("======================================================")); 
  Serial.println(F("Scan for Card and print UID:"));
   //Declare the package descriptor
   constellation.writeInfo("hello world on inter galactical project");
   constellation.declarePackageDescriptor();
}

void loop() { 
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    delay(50);
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    delay(50);
    return;
  }
  // Show some details of the PICC (that is: the tag/card)
  Serial.print(F("Card UID:"));
  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  Serial.println();
}

// Helper routine to dump a byte array as hex values to Serial
void dump_byte_array(byte *buffero, byte bufferSize) {
  delay(50);
  String str[bufferSize];
  String so;
 delay(50);
  for (byte i = 0; i < bufferSize; i++) {
    str[i]= String(buffero[i], HEX);
    delay(500);
    so += str[i];
    delay(500);
  }
  //securite contre les robots
  delay(1000);
  Serial.print(so);
  delay(1000);
  char sochar[256];
  delay(500);
  so.toCharArray(sochar, 256);
  delay(500);
  constellation.pushStateObject("badge", sochar);
  delay(500);
}

