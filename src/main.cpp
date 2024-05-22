#include <ESP32Servo.h>
#include <WiFi.h>
#include <WiFiUdp.h>

#include <stdio.h>
#include <unistd.h>
#include <string>

using namespace std;

const int pinServo = 18; // Pin GPIO al que está conectado el servo
Servo miServo;
int currentAngle = 0;
int speed = 5;

const char* ssid = "Wifi_14000";
const char* pswd = "wifi14000";
const int udpPort = 3000;  // Puerto UDP
WiFiUDP udp;

void receive_packet();
void setDefault();
void moveRight();
void moveLeft();

void setup()
{
  Serial.begin(115200);
  delay(500);

  WiFi.begin(ssid, pswd);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  udp.begin(udpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), udpPort);

  miServo.attach(pinServo);
  setDefault();
}

void loop()
{
  receive_packet();
}

void receive_packet() {
  int packetSize = udp.parsePacket();
  if (packetSize) {
    char packetBuffer[255];
    IPAddress remoteIp = udp.remoteIP();
    int len = udp.read(packetBuffer, 255);

    if (len > 0) packetBuffer[len] = 0;
    Serial.print("From ");    //---|------------------------------------------------|
    Serial.print(remoteIp);   //---| Use this to find out what IP the text is from. |
    Serial.print(" : ");      //---|------------------------------------------------|
    string strCon = packetBuffer;

    Serial.printf("Received packet: %s\n", packetBuffer);

    if(strCon == "mvR") {
      moveRight();
    }
    if(strCon == "mvL") {
      moveLeft();
    }
  }
}

void setDefault() {
  miServo.write(0);
}

void moveRight() {
  if(currentAngle < 180) {
    currentAngle += speed;
    miServo.write(currentAngle);
  }
}

void moveLeft() {
  if(currentAngle > 0) {
    currentAngle -= speed;
    miServo.write(currentAngle);
  }
}