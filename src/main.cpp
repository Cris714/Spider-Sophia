#include <Arduino.h>

#include "spider.h"
#include "wifi_config.h"

int servo_input_pins[6][3] = { 
  { 16, 17, 18 }, // pata 1
  { 20, 21, 22 }, // pata 2
  { 24, 25, 26 }, // pata 3
  {  0,  1,  2 }, // pata 4
  {  4,  5,  6 }, // pata 5
  {  8,  9, 10 }  // pata 6 
};

float servo_home_state_angles[6][3] = {
  { 135, 40, 9 },
  { 135, 72, 1 },
  { 135, 35, 0 },
  { 135, 80, 0 },
  { 135, 45, 1 },
  { 135, 65, 0 }
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
  // for(int i=0; i<3; i++) spider.turn(true);
  // for(int i=0; i<3; i++) spider.turn(false);
  // spider.standup();
  // spider.move_forward();

  string task = wifi_config.receive_packet();

  if(task == "stdup") spider.standup();
  else if(task == "mvFwd") spider.move_forward();
  else if(task == "mvR") spider.turn(true);
  else if(task == "mvL") spider.turn(false);
}

// #include <ESP32Servo.h>

// const int pinServo = 19; // Pin GPIO al que está conectado el servo
// Servo miServo;

// void setup(){
//   miServo.attach(pinServo);
// }

// void loop(){
//   miServo.write(0);
// }

// #include <Wire.h>
// #include <Adafruit_PWMServoDriver.h>
// #include <math.h>

// Adafruit_PWMServoDriver servos = Adafruit_PWMServoDriver(0x40);

// unsigned int pos0=200; // ancho de pulso en cuentas para pocicion 0°
// unsigned int pos180=600; // ancho de pulso en cuentas para la pocicion 180°

// // Configuración para la función sinusoidal
// const float pi = 3.14159;
// const float frequency = 0.2; // Frecuencia de la onda sinusoidal
// const int amplitude = 100;   // Amplitud de la onda (127 porque 255/2 = 127.5)
// const int offset = 375;      // Offset de la onda (128 porque 255/2 = 127.5 + 0.5)
// const int dt = 10;

// void setup() {

//   servos.begin();
//   servos.setPWMFreq(50);
// }

// void setServo(uint8_t servo, int ciclo) { // selección de motor y ancho de pulso
//   servos.setPWM(servo,0,ciclo);
// }
// int pwmValue(int A,float fi,short offset,float f,float t){
//   return A*sin(2*pi*f*t + fi) + offset;
// }

// void loop() {

//   unsigned long currentTime = millis();
  
//   // Calcular el valor de la señal PWM mediante una función sinusoidal
//   float time = currentTime / 1000.0; // Convierte a segundos                              //existe un problema con este código, el tiempo tiene un límite por lo que
//                                                                                           //requerirá modificar por el periodo de la función, así se termina un ciclo
//                                                                                           //y se reinicia, el cual no debe tener una alta presición
//   // Calcular el valor de la señal PWM mediante una función sinusoidal
//   int V0 = pwmValue(amplitude,0,offset,frequency,time);
//   // int V1 = pwmValue(amplitude*0.9,0,offset,frequency*2,time);
//   // int V2 = pwmValue(amplitude*0.8,0,offset,frequency*4,time);
//   // int V3 = pwmValue(amplitude*0.7,0,offset,frequency*6,time);
//   // int V4 = pwmValue(amplitude*0.6,0,offset,frequency/2,time);
//   // int V5 = pwmValue(amplitude*0.5,0,offset,frequency/4,time);
//   // int V6 = pwmValue(amplitude*0.4,0,offset,frequency/6,time);
//   // int V7 = pwmValue(amplitude*0.2,0,offset,frequency/8,time);
//   // setServo(2,V0);
//   // setServo(1,V1);
//   // setServo(2,V2);
//   // setServo(3,V3);
//   // setServo(4,V4);
//   // setServo(5,V5);
//   // setServo(6,V6);
//   // setServo(7,V7);
//   setServo(2, 140);
//   delay(2000);
//   setServo(2, 350);
//   delay(2000);
// }