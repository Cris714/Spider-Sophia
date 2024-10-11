#include <Arduino.h>
#include <sstream>

#include "spider.h"
#include "wifi_config.h"
#include "body_frame_control.h"

#define MAX_INTERVAL 1000

int servo_input_pins[6][3] = { 
    { 16, 17, 18 }, // pata 1
    { 20, 21, 22 }, // pata 2
    { 24, 25, 26 }, // pata 3
    {  0,  1,  2 }, // pata 4
    {  4,  5,  6 }, // pata 5
    {  8,  9, 10 }  // pata 6 
};

float servo_home_state_angles[6][3] = {
    { 135, 53, 10 },
    { 142, 50, 5 },
    { 135, 65, 15 },
    { 135, 56, 0 },
    { 135, 50, 10 },
    { 130, 63, 2 }
};

int servo_min_pulse[6][3] = {
    { 140, 140, 140 },
    { 140, 140, 140 },
    { 140, 140, 140 },
    { 140, 140, 140 },
    { 140, 140, 140 },
    { 140, 140, 140 }
};

int servo_max_pulse[6][3] = {
    { 560, 560, 560 },
    { 560, 560, 560 },
    { 560, 560, 560 },
    { 560, 560, 560 },
    { 560, 560, 560 },
    { 560, 560, 560 }
};

Spider spider(servo_input_pins, servo_home_state_angles, servo_min_pulse, servo_max_pulse);
// char ssid[] = "Maria";
// char pswd[] = "84437158";
char ssid[] = "Wifi_14000";
char pswd[] = "wifi14000";
// char ssid[] = "Kris";
// char pswd[] = "qwer1234";
WifiConfig wifi_config(ssid, pswd);
BodyFrameControl bf_control = BodyFrameControl();

void setup() 
{
    Serial.begin(115200);

    spider.initialize();
    wifi_config.initialize();  
}

// - pata extendida
// hip:  100 
// femur: 60 (aumentar: sube femur) norm: 90
// tibia: 40 (aumentar: baja tibia) norm 120

void loop() 
{
    unsigned long lastPacketTime;
    string task = wifi_config.receive_packet();
    
    if(task != "") {

        switch(task[0]) {
            case 'C':
            {
                stringstream ss (task.substr(1));
                string item;
                float** crds = new float*[1];
                crds[0] = new float[6];
                int** targets = new int*[1];
                targets[0] = new int[6]{1,1,1,1,1,1};

                for (int i=0; i<6; i++) {
                    getline (ss, item, ',');
                    crds[0][i] = stof(item);
                }

                spider.set_coords( bf_control.moveBodyFrame(crds, targets, 1) );

                // Serial.printf("Received %f, %f, %f\n", next_point.p0.x, next_point.p0.y, next_point.p0.z);
                // Serial.printf("Received %f, %f, %f\n", next_point.p1.x, next_point.p1.y, next_point.p1.z);
                // Serial.printf("Received %f, %f, %f\n", next_point.p2.x, next_point.p2.y, next_point.p2.z);
                // Serial.printf("Received %f, %f, %f\n", next_point.p3.x, next_point.p3.y, next_point.p3.z);
                // Serial.printf("Received %f, %f, %f\n", next_point.p4.x, next_point.p4.y, next_point.p4.z);
                // Serial.printf("Received %f, %f, %f\n", next_point.p5.x, next_point.p5.y, next_point.p5.z);
            }
                break;

            case 'W':
            {
                bf_control.move_around(spider, stof(task.substr(1)));
                lastPacketTime = millis();
            }
                break;

            case 'A':
            {
                Serial.print("Caso A :p");
            }
                break;
            default:
                break;
        }
    } else {
        if (bf_control.isMoving() && (millis() - lastPacketTime > MAX_INTERVAL) ) bf_control.finishMovement(spider);
    }
}