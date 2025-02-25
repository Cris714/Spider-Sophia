#include <Arduino.h>
#include <sstream>
#include <vector>

#include "config.h"
#include "secrets.h"
#include "spider.h"
#include "body_frame_control.h"
#include "state.h"
#include "wifi_config.h"


Spider spider(servo_input_pins, servo_home_state_angles, servo_min_pulse, servo_max_pulse);
WifiConfig wifi(SID, PSWD, PORT, MODE);
BodyFrameControl bf_control = BodyFrameControl();
State state = State();

void setup() 
{
    Serial.begin(115200);

    spider.initialize();
    wifi.initialize();  
}

// - pata extendida
// hip:  100 
// femur: 60 (aumentar: sube femur) norm: 90
// tibia: 40 (aumentar: baja tibia) norm 120

void loop() 
{
    string task = wifi.receive_packet();
    
    if(task != "") {

        switch(task[0]) {
            case 'A':
            {


                stringstream ss (task.substr(1));
                string item;

                vector<float> crds;

                while(getline(ss, item, ',')) {
                    crds.push_back(stof(item));
                }

                state.set_linear_speed(0);
                state.set_steady_state(crds);


                // spider.set_coords( bf_control.moveBodyFrame(crds, targets, 1) );

                // t_point6 next_point = bf_control.send_multiple(crds, targets, 1);
                // spider.set_coords(next_point);
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
                string yaw, I, angS;
                getline (ss, yaw, ',');
                getline (ss, I, ',');
                getline(ss, angS, ',');


                state.set_linear_speed(stof(I));
                state.set_angular_speed(stof(angS));
                state.set_yaw(stof(yaw));

                // bf_control.move_around(spider, stof(yaw), stof(I));
                // lastPacketTime = millis();
            }
                break;
            case 'M':
            {
                uint16_t buff[18];
                const char* c_buffer = task.c_str();

                memcpy(buff, &c_buffer[1], 18*sizeof(uint16_t));

                t_point6 next_point = t_point6(
                    t_point(  NTOF(buff[0]),  NTOF(buff[1]),  NTOF(buff[2]) ),
                    t_point(  NTOF(buff[3]),  NTOF(buff[4]),  NTOF(buff[5]) ),
                    t_point(  NTOF(buff[6]),  NTOF(buff[7]),  NTOF(buff[8]) ),
                    t_point(  NTOF(buff[9]), NTOF(buff[10]), NTOF(buff[11]) ),
                    t_point( NTOF(buff[12]), NTOF(buff[13]), NTOF(buff[14]) ),
                    t_point( NTOF(buff[15]), NTOF(buff[16]), NTOF(buff[17]) )
                );

                spider.set_state(next_point);

                Serial.printf("Received %f, %f, %f\n", next_point.p0.x, next_point.p0.y, next_point.p0.z);
                Serial.printf("Received %f, %f, %f\n", next_point.p1.x, next_point.p1.y, next_point.p1.z);
                Serial.printf("Received %f, %f, %f\n", next_point.p2.x, next_point.p2.y, next_point.p2.z);
                Serial.printf("Received %f, %f, %f\n", next_point.p3.x, next_point.p3.y, next_point.p3.z);
                Serial.printf("Received %f, %f, %f\n", next_point.p4.x, next_point.p4.y, next_point.p4.z);
                Serial.printf("Received %f, %f, %f\n", next_point.p5.x, next_point.p5.y, next_point.p5.z);
            }
                break;
            default:
                break;
        }

        // Serial.printf("%f\n", mov.first[1][2]);
        // Serial.printf("%f\n", mov.first[1][1]);
    }

    pair<vector<vector<float>>, vector<vector<int>>> mov = state.next_state();
    spider.set_coords(bf_control.send_multiple(mov.first, mov.second, mov.first.size()));
}