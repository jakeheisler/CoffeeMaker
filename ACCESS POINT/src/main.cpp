#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char *ssid = "CoffeeMaker8266";
const char *pass = "CoffeePassword";

IPAddress ServerIP(192, 168, 4, 1);
IPAddress ClientIP(192, 168, 4, 2);

// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;

int sw1 = 0;
int BS = LOW;
boolean toggle = false;
char packetBuffer[255]; //buffer to hold incoming packet
char  ReplyBuffer[] = "L";
unsigned int localPort = 2000; // local port to listen for UDP packets

//=======================================================================
//                Setup
//=======================================================================
void setup()
{
  Serial.begin(9600);
  Serial.println();
  WiFi.softAP(ssid, pass);    //Create Access point

  //Start UDP
  Serial.println("Starting UDP");
  udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(udp.localPort());

  pinMode(sw1, INPUT);
}
//======================================================================
//                MAIN LOOP
//======================================================================
void loop()
{

  int packetSize = udp.parsePacket();
  if (packetSize) {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    //If serial data is recived send it to UDP
    int len = udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = 0;
    }
    delay(10);
    Serial.println("Contents:");
    Serial.println(packetBuffer);
  }
  delay(10);
  BS = digitalRead(sw1);
  delay(10);


  if (BS == LOW) {
    while (BS == LOW) {
      BS = digitalRead(sw1);
      if (BS == HIGH) {
        break;
      }
      delay(10);
    }

    toggle = !toggle;
    if (toggle == true) {
      strcpy(ReplyBuffer, "\0");
      strcpy(ReplyBuffer, "L");
      delay(10);
    } else {
      strcpy(ReplyBuffer, "\0");
      strcpy(ReplyBuffer, "H");
      delay(10);
    }
    delay(10);

  }

  //Serial.println(ReplyBuffer);
  //data sent to client

  udp.beginPacket(ClientIP, 2000);
  udp.write(ReplyBuffer);
  udp.endPacket();
}
