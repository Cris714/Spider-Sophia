#include <Adafruit_PWMServoDriver.h>

#define PWM_SERVO_DEFAULT_ADDRESS 0x40

class ServoDriver{
    private:
        int freq;
        Adafruit_PWMServoDriver board1 = Adafruit_PWMServoDriver(PWM_SERVO_DEFAULT_ADDRESS + 0);
        Adafruit_PWMServoDriver board2 = Adafruit_PWMServoDriver(PWM_SERVO_DEFAULT_ADDRESS + 1);

    public:
        static inline int angleToPulse(const float angle, const int servomin, const int servomax){ 
            return (servomax - servomin) / 180. * angle + servomin;
        };
        
        ServoDriver(const int freq);
        void initialize();
        void control(const int pin, const float angle, const int min_pulse, const int max_pulse);
};
 
ServoDriver::ServoDriver(const int freq){
    this->freq = freq;
}

void ServoDriver::initialize(){
    board1.begin(); 
    board1.setPWMFreq(freq);  // Analog servos run at ~60 Hz updates

    board2.begin(); 
    board2.setPWMFreq(freq);  // Analog servos run at ~60 Hz updates

    board1.setPWM(15, 0, angleToPulse(90, 5, 575));
}



void ServoDriver::control(const int pin, const float angle, const int min_pulse, const int max_pulse){
    int pulse;
    if (pin/16 == 0){
        pulse = angleToPulse(angle, min_pulse, max_pulse);
        board1.setPWM(pin, 0, pulse);
        // Serial.print(max_pulse); Serial.print(" -> "); Serial.println(angleToPulse(angle, min_pulse, max_pulse)); 
    }
    else
        board2.setPWM(pin%16, 0, angleToPulse(angle, min_pulse, max_pulse));
}