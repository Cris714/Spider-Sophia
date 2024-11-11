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

        string camIP = "";

    public:
        WifiAP(const char* ssid, const char* pswd, const int udpPort);
        void initialize();
        string receive_packet();
        void verifyCamIP(string pckt, string IP);
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
    IPAddress remoteIP;
    
    // descartar paquetes anteriores
    while ((packetSize = udp.parsePacket()) > 0) {
        char packetBuffer[255];
        remoteIP = udp.remoteIP();
        int len = udp.read(packetBuffer, 255);

        if (len > 0) packetBuffer[len] = 0;
        lastPacket = string(packetBuffer); 

        // Serial.printf("Discarded packet from %s: %s\n", remoteIp.toString().c_str(), packetBuffer);
    }

    // último paquete
    if (!lastPacket.empty()) {
        Serial.printf("Received packet from %s: %s\n", remoteIP.toString().c_str(), lastPacket.c_str());

        if(this->camIP.empty()) verifyCamIP(lastPacket, remoteIP.toString().c_str());
    }

    return lastPacket;
}

void WifiAP::verifyCamIP(string pckt, string IP) {
    if(pckt.substr(0,6) == "ESPCAM" && pckt.substr(6) == MAC_ADDR_CAM) {
        this->camIP = IP;
        Serial.printf("CAM IP identified: %s\n", this->camIP);
    }
}