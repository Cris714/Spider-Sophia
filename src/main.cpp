// #include <Arduino.h>

// // put function declarations here:
// int myFunction(int, int);

// void setup() {
//   // put your setup code here, to run once:
//   int result = myFunction(2, 3);
// }

// void loop() {
//   // put your main code here, to run repeatedly:
// }

// // put function definitions here:
// int myFunction(int x, int y) {
//   return x + y;
// }

#include <Arduino.h>
#include <ESP32Servo.h>

const int pinServo = 18; // Pin GPIO al que está conectado el servo
Servo miServo;

void setup() 
{
  miServo.attach(pinServo);
}

void loop() 
{
  miServo.write(0);    // Mover el servo a 0 grados
  delay(1000);         // Esperar 1 segundo
  miServo.write(90);   // Mover el servo a 90 grados
  delay(1000);         // Esperar 1 segundo
  miServo.write(180);  // Mover el servo a 180 grados
  delay(1000);         // Esperar 1 segundo
}