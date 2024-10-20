#include <WiFi.h>
#include <WiFiUdp.h>

#include <string>
using namespace std;

class WifiAP{
    private:
        WiFiUDP udp;
        const char* ssid;
        const char* pswd;
        int udpPort;

    public:
        WifiAP(const char* ssid, const char* pswd, const int udpPort);
        void initialize();
        string receive_packet();
};

WifiAP::WifiAP(const char* ssid, const char* pswd, const int udpPort){
    this->ssid = ssid;
    this->pswd = pswd;
    this->udpPort = udpPort;
}

void WifiAP::initialize(){
    WiFi.softAP(ssid, pswd);
    udp.begin(udpPort);
    Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.softAPIP().toString(), udpPort);
}

string WifiAP::receive_packet() {
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
    
    // descartar paquetes anteriores
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