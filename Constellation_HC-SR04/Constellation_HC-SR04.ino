#include <ESP8266WiFi.h>
//#include <ArduinoJson.h>
#include <Constellation.h>


char ssid[]="Xperia Luc";
char mdp[] = "58Sqyluck23";

Constellation<WiFiClient> constellation("192.168.43.148", 8088, "ESP8266", "MyVirtualPackage", "77d8b4dba50c8a3e4c786b0059d6d1c0afd15d52");

/* Constantes pour le timeout */
const unsigned long MEASURE_TIMEOUT = 25000UL; // 25ms = ~8m à 340m/s

/* Vitesse du son dans l'air en mm/us */
const int TRIGGER = 12; //D6
const int ECHO = 14; //D5

const int TRIGGER2 = 0; //D3
const int ECHO2 = 4; //D2

int defaultValue = -1;
int defaultValue2 = -1;
/********************Pin Definition********************
 *  D0 = 16
 *  D1 = 5;
 *  D2 = 4;
 *  D3 = 0;
 *  D4 = 2;
 *  D5 = 14;
 *  D6 = 12;
 *  D7 = 13;
 *  D8 = 15;
 *  RX = 3;
 *  TX = 1;
 *****************************************************/
void setup(void) {
  pinMode(TRIGGER, OUTPUT);
  digitalWrite(TRIGGER, LOW); // La broche TRIGGER doit être à LOW au repos
  pinMode(ECHO, INPUT);

  pinMode(TRIGGER2, OUTPUT);
  digitalWrite(TRIGGER2, LOW); // La broche TRIGGER doit être à LOW au repos
  pinMode(ECHO2, INPUT);

  Serial.begin(9600);

  // Connect to Wifi  
  WiFi.begin(ssid, mdp);  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // For WiFi101, wait 10 seconds for connection!
  delay(3000);
 
  Serial.println("WiFi connected. IP: ");
  Serial.println(WiFi.localIP());
  // Declare the package descriptor
  constellation.writeInfo("hello world on inter galactical project");
  constellation.declarePackageDescriptor();
}

void pushOnConstellation(){
  int rnd = rand()%100;
 /*if(rnd>90){
        digitalWrite(pin, HIGH);
       // constellation.writeInfo("allumage de la led");
    rnd = 1;
      constellation.pushStateObject("firstObject", stringFormat("{'random':%d, 'combien':%d}", rnd, 25));

   delay(1000);
  }else{
    digitalWrite(pin, LOW);
    rnd = 0;
      constellation.pushStateObject("firstObject", stringFormat("{'random':%d, 'combien':%d}", rnd, 25));

    delay(500);
  }*/
}

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
    defaultValue = distanceMeasured;
        Serial.print("default value : ");
        Serial.println(defaultValue);

  }
  if((distanceMeasured > defaultValue + 3)||(distanceMeasured < defaultValue - 3)){
    /* Affiche les résultats en mm, cm et m */
    Serial.print(" Distance: ");
    Serial.print(distanceMeasured);
    Serial.print(" cm ");
    Serial.println("");
  }
  delay(50);
  digitalWrite(TRIGGER2, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER2, LOW);
  
  /* 2. Mesure le temps entre l'envoi de l'impulsion ultrasonique et son écho (si il existe) */
  long measure2 = pulseIn(ECHO2, HIGH);
   
  /* 3. Calcul la distance à partir du temps mesuré */
  long distanceMeasured2 = measure2 / 58.2;
  if(defaultValue2 == -1){
    defaultValue2 = distanceMeasured2;
            Serial.print("default value : ");
        Serial.println(defaultValue2);
  }
  if((distanceMeasured2 > defaultValue2 + 3)||(distanceMeasured2 < defaultValue2 - 3)){
    /* Affiche les résultats en mm, cm et m */
    Serial.print(" Distance2: ");
    Serial.print(distanceMeasured2);
    Serial.print(" cm ");
    Serial.println("");
  }

   
  /* Délai d'attente pour éviter d'afficher trop de résultats à la seconde */
  delay(50);
  pushOnConstellation();
  constellation.loop();
  }

