#include <Arduino.h>
#include <sstream>

#include "config.h"
#include "spider.h"
#include "wifi_config.h"
#include "body_frame_control.h"

Spider spider(servo_input_pins, servo_home_state_angles, servo_min_pulse, servo_max_pulse);
WifiConfig wifi_config(SID, PSWD);
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
                stringstream ss (task.substr(1));
                string yaw, I;
                getline (ss, yaw, ',');
                getline (ss, I, ',');

                bf_control.move_around(spider, stof(yaw), stof(I));
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
        if (bf_control.isMoving() && (millis() - lastPacketTime > MAX_INTERVAL * 1000) ) bf_control.finishMovement(spider);
    }
}