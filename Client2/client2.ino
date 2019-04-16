
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char *ssid = "CoffeeMaker8266";
const char *pass = "CoffeePassword";

unsigned int localPort = 2000; // local port to listen for UDP packets

IPAddress ServerIP(192, 168, 4, 1);
IPAddress ClientIP(192, 168, 4, 2);

// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;

char packetBuffer[255]; //buffer to hold incoming packet
char  ReplyBuffer[] = "Client";

int steppin = 4;
int dirpin = 0;
int BUFF = 0;
//======================================================================
//                Setup
//======================================================================
void setup()
{

  Serial.begin(9600);
  Serial.println();

  WiFi.begin(ssid, pass);   //Connect to access point

  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //Start UDP
  Serial.println("Starting UDP");
  udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(udp.localPort());

  pinMode(steppin, OUTPUT);
  pinMode(dirpin, OUTPUT);
}
//======================================================================
//                MAIN LOOP
//======================================================================
void loop()
{
  int packetSize = udp.parsePacket();
  if (packetSize) {
    //Serial.print("Received packet of size ");
    //Serial.println(packetSize);
    //If serial data is recived send it to UDP
    int len = udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = 0;
    }
    delayMicroseconds(10);
    Serial.println("Contents:");
    Serial.println(packetBuffer);
  }
  BUFF = packetBuffer[0];

  if (BUFF == 'H')
  {
    digitalWrite(dirpin, HIGH);
    digitalWrite(steppin, LOW);
    delayMicroseconds(450);
    digitalWrite(steppin, HIGH);
    delayMicroseconds(450);
  } else {
    digitalWrite(dirpin, LOW);
    digitalWrite(steppin, LOW);
    delayMicroseconds(400);
    digitalWrite(steppin, HIGH);
    delayMicroseconds(400);
  }
  udp.beginPacket(ServerIP, 2000);
  udp.write(ReplyBuffer);
  udp.endPacket();
}
