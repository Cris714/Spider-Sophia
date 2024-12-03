#include <WiFi.h>
#include <WiFiUdp.h>

#include <string>
using namespace std;

class WifiConfig{
    private:
        WiFiUDP udp;
        const char* ssid;
        const char* pswd;
        string mode;
        uint16_t udpPort;

        string camIP = "";
        string appIP = "";

    public:
        WifiConfig(const char* ssid, const char* pswd, const int udpPort, string mode);
        void initialize();
        string receive_packet();
        void send_packet(string msg, string IP);
        void verifyDeviceIP(string pckt, string IP);
};

WifiConfig::WifiConfig(const char* ssid, const char* pswd, const int udpPort, string mode = "AP"){
    this->ssid = ssid;
    this->pswd = pswd;
    this->udpPort = udpPort;
    this->mode = mode; // "AP" -> WifiEsp, "WIFI" -> Cualquier otra red
}

void WifiConfig::initialize(){
    if(this->mode == "AP") {
        WiFi.softAP(ssid, pswd);
        udp.begin(udpPort);
        Serial.printf("Mode AP | Now listening at IP %s, UDP port %d\n", WiFi.softAPIP().toString(), udpPort);
    } else if(this->mode == "WIFI") {
        WiFi.begin(ssid, pswd);
        while (WiFi.status() != WL_CONNECTED) {
            delay(1000);
                Serial.println("Connecting to WiFi...");
            }
            Serial.println("Connected to WiFi");

            udp.begin(udpPort);
            Serial.printf("Mode WIFI | Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), udpPort);
        }
}

string WifiConfig::receive_packet() {
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

        if(this->appIP.empty() || (!this->appIP.empty() && remoteIP.toString().c_str() == this->appIP)) lastPacket = string(packetBuffer); 
        verifyDeviceIP(string(packetBuffer), remoteIP.toString().c_str());


        // Serial.printf("Discarded packet from %s: %s\n", remoteIp.toString().c_str(), packetBuffer);
    }

    // último paquete
    if (!lastPacket.empty()) {
        Serial.printf("Received packet from %s: %s\n", remoteIP.toString().c_str(), lastPacket.c_str());
    }

    return lastPacket;
}

void WifiConfig::send_packet(string msg, string IP) {
    Serial.printf("Sending packet to %s: %s\n", IP.c_str(), msg.c_str());
    udp.beginPacket(IP.c_str(), udpPort);
    udp.print(msg.c_str());
    udp.endPacket();
}

void WifiConfig::verifyDeviceIP(string pckt, string IP) {
    // Serial.printf("%s\n", pckt.c_str());
    // SET ESP CAM IP
    if(this->camIP.empty() && pckt.substr(0,6) == "ESPCAM" && pckt.substr(6) == MAC_ADDR_CAM) {
        this->camIP = IP;
        Serial.printf("CAM IP identified: %s\n", this->camIP.c_str());
    }
    // CONNECT APP IP
    if(this->appIP.empty()) {
        if(pckt[0] == 'C') {
            this->appIP = IP;
            Serial.printf("APP IP connected: %s\n", this->appIP.c_str());
            send_packet("A", IP);
        }
    } else  {
        if(pckt[0] == 'C') { // Reject Connection
            send_packet("R", IP);
        }
        if(pckt[0] == 'D' && this->appIP == IP) { // Disconnect APP IP
            this->appIP = "";
            Serial.println("APP IP disconnected...");
        }
    }

}