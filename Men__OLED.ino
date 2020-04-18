#include <Wire.h>  
#include <SPI.h>
#include "OLEDDisplayUi.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SEALEVELPRESSURE_HPA (1013.25)
#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
const int SDA_PIN = D1;
const int SDC_PIN = D2;
Adafruit_BME280 bme;
#define OLED_RESET -1
Adafruit_SSD1306 display1(OLED_RESET);
Adafruit_SSD1306 display2(OLED_RESET);
int buttonPin1 = D7;
int buttonPin2 = D5;
int buttonPin3 = D6; 
int buttonState1 = LOW;
int buttonState2 = LOW;
int buttonState3 = LOW;
int Zaehler = 1;
int Zaehler2 = 1;
int Enter = 0;
int Menu = 0;
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// WIFI
const char* ssid = "*********8";                      //SSID WLAN (WLAN Name)
const char* password = "************";              //WLAN Passwort
const char* mqtt_server = "**********";           //Adresse vom MQTT Server

String _DisplayLine1 = "";                         //Erzeugt einen String mit dem Namen _DisplayLine1
String _DisplayLine2 = "";
String _DisplayLine3 = "";
String _DisplayLine4 = "";
String _DisplayLine5 = "";
String _DisplayLine6 = "";
String _DisplayLine7 = "";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;                                   // long = für erweiterte Nummernspeicher ; lastMsg = Variablenname ; 0 = Wert der Variable
char msg[50];                                       // zum Darstellen einer Zeichkette aus einem Array  
int value = 0;                                      // int = für Ganzzahlentyp ; value = Variablenname; 0 = Wert der zugewiesen wird
int buttonPushCounter = 0;   // counter for the number of button presses
int lastButtonState = 0;

//-------------------------------------------------Setup-----------------------------------------------------------------------------------------------------------

void setup()   {
  Serial.begin(9600);                               // startet die Serielle Verbindung für den Monitor
    display1.begin(SSD1306_SWITCHCAPVCC, 0x3D);      // startet das Display 1
    display2.begin(SSD1306_SWITCHCAPVCC, 0x3C);      // startet das Display 2
    WiFi.begin(ssid, password);                       // 
 
    while (WiFi.status() != WL_CONNECTED) {             //solange (Wifi.status() ungleich Verbunden)
    delay(500);                                         // pause(500ms)
    Serial.print(".");
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback); 
    pinMode(buttonPin2, INPUT);  
    pinMode(buttonPin3, INPUT);
    pinMode(buttonPin1, INPUT);
    unsigned status;  
    status = bme.begin(0x76);  
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
        Serial.println("-- Default Test --");
        Serial.println();
    }
  }

}
//-------------------------- Abruf der Proxmox Daten der MQTT vom IO-Broker----------------------------------------------------------------------------------
                                                                                            // callback = Rückruf / char* topic = Themabereich komplett abfrage ( display_line1 ...bis display_line4)
void callback(char* topic, byte* payload, unsigned int length) {                            // topic = Thema,                    , unsigned int = vorzeichenlose Ganzzahlen

if ( strcmp( topic, "display_line1" ) == 0 )                                                //vergleiche String topic ob "display_line1" vorhanden wenn ja dann Ergebnis 0
{
_DisplayLine1 = "";
for (int i = 0; i < length; i++) { _DisplayLine1 += (char)payload[i]; }                   // "+=" = Variable mit Konstante oder anderer Variable addieren   
}    // i kleiner länge; addiere einen Speicher dazu / (Datentyp char)payload[arrayinhalt];
 
if ( strcmp( topic, "display_line2" ) == 0 )                            // strcmp = String Compare (String1 , String2) / "==0" heisst beide Strings sind gleich 
{
_DisplayLine2 = "";
for (int i = 0; i < length; i++) { _DisplayLine2 += (char)payload[i]; }
}
 
if ( strcmp( topic, "display_line3" ) == 0 )
{
_DisplayLine3 = "";
for (int i = 0; i < length; i++) { _DisplayLine3 += (char)payload[i]; }
}
 
if ( strcmp( topic, "display_line4" ) == 0 )
{
_DisplayLine4 = "";
for (int i = 0; i < length; i++) { _DisplayLine4 += (char)payload[i]; }
}

if ( strcmp( topic, "display_line5" ) == 0 )
{
_DisplayLine5 = "";
for (int i = 0; i < length; i++) { _DisplayLine5 += (char)payload[i]; }
}

if ( strcmp( topic, "display_line6" ) == 0 )
{
_DisplayLine6 = "";
for (int i = 0; i < length; i++) { _DisplayLine6 += (char)payload[i]; }
}
if ( strcmp( topic, "display_line7" ) == 0 )
{
_DisplayLine7 = "";
for (int i = 0; i < length; i++) { _DisplayLine7 += (char)payload[i]; }
}


//Proxmox_Systemstatus();
}

void reconnect() {
// Loop until we're reconnected
while (!client.connected()) {
Serial.print("Attempting MQTT connection...");
// Create a random client ID
String clientId = "ESP8266Client-";
clientId += String(random(0xffff), HEX);

if (client.connect(clientId.c_str())) {
    Serial.println("connected");
    client.subscribe("display_line1");
    client.subscribe("display_line2");
    client.subscribe("display_line3");
    client.subscribe("display_line4");
    client.subscribe("display_line5");
    client.subscribe("display_line6");
    client.subscribe("display_line7");   
}

 else {
    Serial.print("failed, rc=");
    Serial.print(client.state());
    Serial.println(" try again in 5 seconds");
}
}
}

//------------------------------------------------------------------Hauptmenüs------------------------------------------------
void Auswahl_Uebersicht(){
  
    display1.clearDisplay();
    display1.setTextColor(WHITE);
    display1.setTextSize(1);
    display1.setCursor(27, 1);  display1.print(" Hauptmenue ");
    display1.drawLine(32, 9, 92, 9, WHITE);
    display1.setCursor(3, 15); display1.print("  Uebersicht");
    display1.setCursor(3, 29); display1.print("  Proxmox");
    display1.setCursor(3, 43); display1.print("  Temperatur");
    display1.setCursor(3, 57); display1.print("  >> Seite 2");
    display1.setCursor(3, (Zaehler * 14) + 1);
    display1.println(">");
    display1.display();
    if(Zaehler == 1){
       Uebersicht();
    }
    if(Zaehler == 2){
      Proxmox_Info();
    }
    if(Zaehler == 3){
      Temperatur_Info();
    }
   
    if(Zaehler == 2 && buttonState3 ==1){
      Menu = Menu + 1;
      Zaehler2 = 1;
    }
    if(Zaehler == 3 && buttonState3 ==1){
      Menu = Menu + 2;
      Zaehler2 = 1;
    }
     
}
//---------------------------------------------------------------------------------------------------------------------------------
void Auswahl_Proxmox(){
    display1.clearDisplay();
    display1.setTextColor(WHITE);
    display1.setCursor(40, 1);  display1.print("Proxmox");
    display1.drawLine(40, 9, 81, 9, WHITE);
    display1.setCursor(3, 15); display1.print("  Systeminfo");
    display1.setCursor(3, 29); display1.print("  Systemstatus");
    display1.setCursor(3, 43); display1.print("  VM Info");
    display1.setCursor(3, 57); display1.print("  Zurueck");
    display1.setCursor(3, (Zaehler2 * 14) + 1); display1.println(">");
    display1.display();
    if(Zaehler2 == 1){
      Proxmox_Seite1();
    }
    if(Zaehler2 == 2){
     Proxmox_Systemstatus();
     }
   
    if(Zaehler2 == 3){
      VM_Status();
      }
    if(Zaehler2 == 4 && buttonState3 ==1){
      Auswahl_Uebersicht();
      Menu = 0;
     }
    }

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Auswahl_Temperatur(){

    display1.clearDisplay();
    display1.setTextColor(WHITE);
    display1.setCursor(27, 1);  display1.print(" Temperatur ");
    display1.drawLine(32, 9, 92, 9, WHITE);
    display1.setCursor(3, 15); display1.print("  Wohnzimmer");
    display1.setCursor(3, 29); display1.print("  Balkon");
    display1.setCursor(3, 43); display1.print("  Kinderzimmer");
    display1.setCursor(3, 57); display1.print("  Zurueck");
    display1.setCursor(3, (Zaehler2 * 14) + 1); display1.println(">");
    display1.display();
    if(Zaehler2 == 1){
      Temperatur();}
    if(Zaehler2 == 2){
      Temperatur_Balkon();  
    }
     if(Zaehler2 == 4 && buttonState3 ==1){
      Auswahl_Uebersicht();
      Menu = 0;
     
    }
    
  }

//-------------------------------Darstellungen auf Display 2--------------------------------------------------------------------
void Uebersicht(){
    
    display2.clearDisplay();
    display2.drawRect(0, 0, 128, 64, WHITE);
    display2.setTextColor(WHITE);
    display2.setCursor(10, 2); display2.print("Systeminformation");
    display2.setCursor(3, 20); display2.print("Einfaches Infosystem");
    display2.setCursor(3, 30); display2.print("zum Abrufen diverser");
    display2.setCursor(3, 40); display2.print("Daten vom SmartHome");
    display2.setCursor(3, 50); display2.print("und derer Komponenten.");
    display2.display(); 
  }

void Proxmox_Info(){
    display2.clearDisplay();
    display2.drawRect(0, 0, 128, 64, WHITE);
    display2.setTextColor(WHITE);
    display2.setCursor(29, 2); display2.print("Proxmox-Info");
    display2.setCursor(3, 20); display2.print("Abrufen vom System-");
    display2.setCursor(3, 30); display2.print("zustand und einzelner");
    display2.setCursor(3, 40); display2.print("Auslastungswerte der ");
    display2.setCursor(3, 50); display2.print("Virtuellen Maschinen");
    display2.display();
}

void Temperatur_Info(){
    display2.clearDisplay();
    display2.drawRect(0, 0, 128, 64, WHITE);
    display2.setTextColor(WHITE);
    display2.setCursor(28, 2); display2.print("Temperatur-Info");
    display2.setCursor(3, 20); display2.print("Abrufen der einzelnen");
    display2.setCursor(3, 30); display2.print("Temperaturwerte usw.");
    display2.setCursor(3, 40); display2.print("von diversen Sensoren ");
    display2.setCursor(3, 50); display2.print("in diversen Räumen");
    display2.display();
}

void Temperatur() {
  
    display2.clearDisplay();
    display2.drawRect(0, 0, 128, 64, WHITE);
    display2.setTextColor(WHITE);
    display2.setCursor(30, 1); display2.print("Wohnzimmer");
    display2.setCursor(3, 10);    display2.print("Temperatur: ");
    display2.setCursor(74, 10);   display2.print(bme.readTemperature());
    display2.setCursor(104, 10);  display2.print(" C");
    display2.setCursor(3, 25);   display2.print("Luftfeuchte: ");
    display2.setCursor(74, 25);  display2.print(bme.readHumidity());
    display2.setCursor(104, 25); display2.println(" %");
    display2.setCursor(3, 40);   display2.print("Luftdruck: ");
    display2.setCursor(74, 40);  display2.print(bme.readPressure() / 100.0F);
    display2.setCursor(104, 40); display2.println(" hPa");
    display2.setCursor(3, 55);   display2.print("Hoehenmeter: ");
    display2.setCursor(74, 55);  display2.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    display2.setCursor(104, 55); display2.println(" m");
    display2.display();
    delay(100);
    }

  void Temperatur_Balkon() {
  
    display2.clearDisplay();
    display2.drawRect(0, 0, 128, 64, WHITE);
    display2.setTextColor(WHITE);
    display2.setCursor(30, 1); display2.print("Balkon");
    display2.setCursor(3, 10);    display2.print("Temperatur: ");
    display2.setCursor(104, 10);  display2.print(" C");
    display2.setCursor(3, 25);   display2.print("Luftfeuchte: ");
    display2.setCursor(104, 25); display2.println(" %");
    display2.setCursor(3, 40);   display2.print("Luftdruck: ");
    display2.setCursor(104, 40); display2.println(" hPa");
    display2.setCursor(3, 55);   display2.print("Hoehenmeter: ");
    display2.setCursor(104, 55); display2.println(" m");
    display2.display();
    delay(100);
    }  

void Proxmox_Seite1(){
  
   display2.clearDisplay();
   display2.setTextColor(WHITE);
   display2.setCursor(10, 1); display2.print("Proxmox Hauptmenue");
   display2.drawLine(10, 9, 117, 9, WHITE);
   display2.println();
   display2.setCursor(2, 20); display2.print("CPU: Intel Nuc J3455");
   display2.setCursor(2, 30); display2.print("     Celeron 4x2,3Ghz");
   display2.setCursor(2, 40); display2.print("RAM: 2x4GB DDR3 ");
   display2.setCursor(2, 50); display2.print("HDD: 128GB SSD ");
   display2.display(); 
}

void Proxmox_Systemstatus(){
  
display2.clearDisplay();
display2.setTextColor(WHITE);
display2.setCursor(3, 5);    display2.print("CPU Last:   ");
display2.setCursor(90, 5);   display2.print(_DisplayLine1.c_str());
display2.setCursor(108, 5);  display2.print(" %");
display2.setCursor(3, 20);   display2.print("Freier RAM:  ");
display2.setCursor(80, 20);  display2.print(_DisplayLine2.c_str());
display2.setCursor(104, 20); display2.print(" Mb");
display2.setCursor(3, 35);   display2.print("Systemstat:");
display2.setCursor(80, 35);  display2.print(_DisplayLine3.c_str());
display2.setCursor(3, 50);   display2.print("Core Temp:   ");
display2.setCursor(98, 50);  display2.print(_DisplayLine4.c_str());
display2.setCursor(108, 50); display2.print(" C");
display2.display();
client.loop();
}

void VM_Status(){
  
display2.clearDisplay();
display2.setTextColor(WHITE);
display2.setCursor(40, 1); display2.print("Status VM");
display2.drawLine(40, 9, 95, 9, WHITE);
display2.setCursor(3, 20);    display2.print("IOBroker:  ");
display2.setCursor(80, 20);   display2.print(_DisplayLine5.c_str());
display2.setCursor(3, 35);   display2.print("Ubuntu:    ");
display2.setCursor(80, 35);  display2.print(_DisplayLine6.c_str());
display2.setCursor(3, 50);   display2.print("Testsyst:  ");
display2.setCursor(80, 50);  display2.print(_DisplayLine7.c_str());
display2.display();
client.loop();
}



void loop() {
  
  buttonState1 = digitalRead(buttonPin2);
  buttonState2 = digitalRead(buttonPin3);
  buttonState3 = digitalRead(buttonPin1);
if (!client.connected()) {
reconnect();
}

//--------------------------------------Auswahl Menü mit 3 Möglichkeiten---------------------------------------------
     if(buttonState1 == 1){
      delay(500);
      if(buttonState1 == 1 && Zaehler < 3){
        Zaehler = Zaehler +1;
               }
                else if(buttonState1 == 1 && Zaehler ==3)
                {
                  Zaehler = Zaehler -2;
                }
                Zaehler = Zaehler;                       
      }
            
      if(buttonState2 == 1){
        delay(500);
        if(buttonState2 == 1 && Zaehler > 1){
        Zaehler = Zaehler -1;
           }
           else if(buttonState2 == 1 && Zaehler ==1)
                {
                  Zaehler = Zaehler +2;
                }
          Zaehler = Zaehler;
      }
//--------------------------------------Auswahl Menü mit 3 Möglichkeiten---------------------------------------------
 if(buttonState1 == 1){
      delay(500);
      if(buttonState1 == 1 && Zaehler2 < 4){
        Zaehler2 = Zaehler2 +1;
               }
                else if(buttonState1 == 1 && Zaehler2 ==4)
                {
                  Zaehler2 = Zaehler2 -3;
                }
                Zaehler2 = Zaehler2;                       
      }
            
      if(buttonState2 == 1){
        delay(500);
        if(buttonState2 == 1 && Zaehler2 > 1){
        Zaehler2 = Zaehler2 -1;
           }
           else if(buttonState2 == 1 && Zaehler2 ==1)
                {
                  Zaehler2 = Zaehler2 +3;
                }
          Zaehler2 = Zaehler2;
      }

//-----------------------------------Auswahl der Untermenüs-----------------------------------------------------------------------------

      
      if(Menu == 0){
         Auswahl_Uebersicht();          
      }
      if(Menu == 1){
      Auswahl_Proxmox();
      }
      if(Menu == 2){
        Auswahl_Temperatur();
      }
     
}       
