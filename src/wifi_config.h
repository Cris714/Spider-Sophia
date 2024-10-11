#include <WiFi.h>
#include <WiFiUdp.h>

#include <string>
using namespace std;

char replyPacket[] = "Connected";

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
    // udp.flush();
    // int packetSize = udp.parsePacket();
    // if (packetSize) {
    //     char packetBuffer[255];
    //     IPAddress remoteIp = udp.remoteIP();
    //     int len = udp.read(packetBuffer, 255);

    //     if (len > 0) packetBuffer[len] = 0;
    //     Serial.print("From ");    //---|------------------------------------------------|
    //     Serial.print(remoteIp);   //---| Use this to find out what IP the text is from. |
    //     Serial.print(" : ");      //---|------------------------------------------------|
    //     string strCon = packetBuffer;

    //     Serial.printf("Received packet: %s\n", packetBuffer);

    //     return strCon;
    // }
    // return "";

    string lastPacket = "";
    int packetSize;
    
    // desacrtar paquetes anteriores
    while ((packetSize = udp.parsePacket()) > 0) {
        char packetBuffer[255];
        IPAddress remoteIp = udp.remoteIP();
        int len = udp.read(packetBuffer, 255);

        if (len > 0) packetBuffer[len] = 0;
        lastPacket = string(packetBuffer); 

        // Serial.printf("Discarded packet from %s: %s\n", remoteIp.toString().c_str(), packetBuffer);
    }

    // último paquete
    if (!lastPacket.empty()) {
        Serial.printf("Received packet: %s\n", lastPacket.c_str());
    }

    return lastPacket;
}