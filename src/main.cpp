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

const int pinServo1 = 18; // Pin GPIO al que está conectado el servo
const int pinServo2 = 19; // Pin GPIO al que está conectado el servo
Servo miServo1;
Servo miServo2;

#define N_SAMPLES 53

float angles[N_SAMPLES][2] = {{96.598,53.021},{95.97,50.254},{95.823,48.168},{96.235,46.898},{97.261,46.551},{98.914,47.156},{101.149,48.661},{103.88,50.946},{107.005,53.862},{110.422,57.265},{114.044,61.032},{117.797,65.06},{121.621,69.271},{125.467,73.599},{129.291,77.994},{133.053,82.413},{136.718,86.816},{140.249,91.17},{143.61,95.441},{146.763,99.596},{149.668,103.6},{152.281,107.418},{154.559,111.01},{156.454,114.337},{157.919,117.353},{158.911,120.016},{159.389,122.278},{159.323,124.098},{158.7,125.436},{157.522,126.262},{155.814,126.556},{153.623,126.309},{151.014,125.529},{148.065,124.235},{144.863,122.456},{141.49,120.23},{138.024,117.601},{134.532,114.613},{131.067,111.312},{127.671,107.741},{124.374,103.942},{121.198,99.952},{118.156,95.809},{115.257,91.547},{112.507,87.199},{109.91,82.799},{107.471,78.38},{105.197,73.981},{103.098,69.645},{101.19,65.422},{99.498,61.374},{98.057,57.581},{96.917,54.141}};

void setup() 
{
  miServo1.attach(pinServo1);
  miServo2.attach(pinServo2);
}

void loop() 
{
  for (int i = 0; i < N_SAMPLES; i++){
    miServo1.write(angles[i][0]);   
    miServo2.write(angles[i][1]);  
    delay(50);
  }
  for (int i = N_SAMPLES - 1; i >= 0; i--){
    miServo1.write(angles[i][0]);   
    miServo2.write(angles[i][1]);  
    delay(50);
  }
}