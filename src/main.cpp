#include <Arduino.h>

#include "spider.h"
 
 

int servo_input_pins[6][3] = { 
  { 0,  6, 16 }, // pata 1
  { 1,  7, 12 }, // pata 2
  { 2,  8, 13 }, // pata 3
  { 3,  9, 14 }, // pata 4
  { 4, 10, 15 }, // pata 5
  { 5, 11, 17 }  // pata 6 
};

float servo_home_state_angles[6][3] = {
  { 60, 90, 120 },
  { 60, 90, 120 },
  { 60, 90, 120 },
  { 60, 90, 120 },
  { 60, 90, 120 },
  { 60, 90, 120 }
};

Spider spider(servo_input_pins, servo_home_state_angles);

int angleToPulse(int ang);

void setup() 
{
  Serial.begin(9600);
}

// - pata extendida
// hip:  100 
// femur: 60 (aumentar: sube femur) norm: 90
// tibia: 40 (aumentar: baja tibia) norm 120

void loop() 
{
  spider.home_position();
}

