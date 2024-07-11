#include <WiFi.h>
#include <WiFiUdp.h>

#include <string>
using namespace std;

class WifiConfig{
    private:
        WiFiUDP udp;
        char* ssid;
        char* pswd;
        const int udpPort = 3000;

    public:
        WifiConfig(char* ssid, char* pswd);
        void initialize();
        string receive_packet();
};

WifiConfig::WifiConfig(char* ssid, char* pswd){
    this->ssid = ssid;
    this->pswd = pswd;
}

void WifiConfig::initialize(){
    WiFi.begin(ssid, pswd);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");

    udp.begin(udpPort);
    Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), udpPort);
}

string WifiConfig::receive_packet() {
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

    return strCon;
  }
  return "";
}