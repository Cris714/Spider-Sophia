#include <Arduino.h>

#include "spider.h"
#include "wifi_config.h"
#include "body_frame_control.h"

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
char ssid[] = "Wifi_14000";
char pswd[] = "wifi14000";
WifiConfig wifi_config(ssid, pswd);
BodyFrameControl bf_control = BodyFrameControl();

void setup() 
{
    Serial.begin(115200);

    spider.initialize();
    // wifi_config.initialize();  
}

// - pata extendida
// hip:  100 
// femur: 60 (aumentar: sube femur) norm: 90
// tibia: 40 (aumentar: baja tibia) norm 120

void loop() 
{
    // for(int i=0; i<3; i++) spider.turn(true);
    // for(int i=0; i<3; i++) spider.turn(false);
    // spider.standup();
    // spider.move_forward();

    // string task = wifi_config.receive_packet();
    string task = "prueba";

    if(task == "prueba") {
        MatrixXf points = bf_control.moveBodyFrame(0,1,0,radians(0),radians(20),radians(0));

        t_point6 next_point = t_point6(
            t_point( points(0, 0), points(0, 1), points(0,2) ),
            t_point( points(1, 0), points(1, 1), points(1,2) ),
            t_point( points(2, 0), points(2, 1), points(2,2) ),
            t_point( points(3, 0), points(3, 1), points(3,2) ),
            t_point( points(4, 0), points(4, 1), points(4,2) ),
            t_point( points(5, 0), points(5, 1), points(5,2) )
        );

        spider.set_state(next_point);

        Serial.printf("Received %f, %f, %f\n", next_point.p0.x, next_point.p0.y, next_point.p0.z);
        Serial.printf("Received %f, %f, %f\n", next_point.p1.x, next_point.p1.y, next_point.p1.z);
        Serial.printf("Received %f, %f, %f\n", next_point.p2.x, next_point.p2.y, next_point.p2.z);
        Serial.printf("Received %f, %f, %f\n", next_point.p3.x, next_point.p3.y, next_point.p3.z);
        Serial.printf("Received %f, %f, %f\n", next_point.p4.x, next_point.p4.y, next_point.p4.z);
        Serial.printf("Received %f, %f, %f\n", next_point.p5.x, next_point.p5.y, next_point.p5.z);

        delay(5000);
    }

    // if(task != ""){
    //     const char* c_buffer = task.c_str();

    //     switch (c_buffer[0]) {
    //         case 'A':
    //         {
    //             uint16_t buff[18];

    //             memcpy(buff, &c_buffer[1], 18*sizeof(uint16_t));

    //             t_point6 next_point = t_point6(
    //                 t_point(  NTOF(buff[0]),  NTOF(buff[1]),  NTOF(buff[2]) ),
    //                 t_point(  NTOF(buff[3]),  NTOF(buff[4]),  NTOF(buff[5]) ),
    //                 t_point(  NTOF(buff[6]),  NTOF(buff[7]),  NTOF(buff[8]) ),
    //                 t_point(  NTOF(buff[9]), NTOF(buff[10]), NTOF(buff[11]) ),
    //                 t_point( NTOF(buff[12]), NTOF(buff[13]), NTOF(buff[14]) ),
    //                 t_point( NTOF(buff[15]), NTOF(buff[16]), NTOF(buff[17]) )
    //             );
                
    //             spider.set_state(next_point);
                
    //             // Serial.printf("Received %f, %f, %f\n", next_point.p0.x, next_point.p0.y, next_point.p0.z);
    //             // Serial.printf("Received %f, %f, %f\n", next_point.p1.x, next_point.p1.y, next_point.p1.z);
    //             // Serial.printf("Received %f, %f, %f\n", next_point.p2.x, next_point.p2.y, next_point.p2.z);
    //             // Serial.printf("Received %f, %f, %f\n", next_point.p3.x, next_point.p3.y, next_point.p3.z);
    //             // Serial.printf("Received %f, %f, %f\n", next_point.p4.x, next_point.p4.y, next_point.p4.z);
    //             // Serial.printf("Received %f, %f, %f\n", next_point.p5.x, next_point.p5.y, next_point.p5.z);
    //         }
    //             break;

    //         case 'C':
    //         {
    //             uint16_t buff[18];

    //             memcpy(buff, &c_buffer[1], 18*sizeof(uint16_t));

    //             t_point6 next_point = t_point6(
    //                 t_point(  NTOF(buff[0]),  NTOF(buff[1]),  NTOF(buff[2]) ),
    //                 t_point(  NTOF(buff[3]),  NTOF(buff[4]),  NTOF(buff[5]) ),
    //                 t_point(  NTOF(buff[6]),  NTOF(buff[7]),  NTOF(buff[8]) ),
    //                 t_point(  NTOF(buff[9]), NTOF(buff[10]), NTOF(buff[11]) ),
    //                 t_point( NTOF(buff[12]), NTOF(buff[13]), NTOF(buff[14]) ),
    //                 t_point( NTOF(buff[15]), NTOF(buff[16]), NTOF(buff[17]) )
    //             );
                
    //             spider.set_coords(next_point);
                
    //             // Serial.printf("Received %f, %f, %f\n", next_point.p0.x, next_point.p0.y, next_point.p0.z);
    //             // Serial.printf("Received %f, %f, %f\n", next_point.p1.x, next_point.p1.y, next_point.p1.z);
    //             // Serial.printf("Received %f, %f, %f\n", next_point.p2.x, next_point.p2.y, next_point.p2.z);
    //             // Serial.printf("Received %f, %f, %f\n", next_point.p3.x, next_point.p3.y, next_point.p3.z);
    //             // Serial.printf("Received %f, %f, %f\n", next_point.p4.x, next_point.p4.y, next_point.p4.z);
    //             // Serial.printf("Received %f, %f, %f\n", next_point.p5.x, next_point.p5.y, next_point.p5.z);
    //         }
    //             break;

    //         default:
    //             break;
    //     }
    // }

    // if(task == "stdup") spider.standup();
    // else if(task == "mvFwd") spider.move_forward();
    // else if(task == "mvR") spider.turn(true);
    // else if(task == "mvL") spider.turn(false);
}