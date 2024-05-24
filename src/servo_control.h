#include <Adafruit_PWMServoDriver.h>

#define PWM_SERVO_DEFAULT_ADDRESS 0x40
#define SERVOMIN  10 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  775 // this is the 'maximum' pulse length count (out of 4096)

class ServoDriver{
    private:
        int n_boards;
        Adafruit_PWMServoDriver *boards;
    public:
        static inline uint16_t angleToPulse(const float angle){ 
            return map(angle, 0, 180, SERVOMIN, SERVOMAX);
        };
        
        ServoDriver(const int n_boards, const int freq);
        void control(const int pin, const float angle);
};

ServoDriver::ServoDriver(const int n_boards, const int freq){
    /// wal
    this->n_boards = n_boards;
    boards = new Adafruit_PWMServoDriver[n_boards];

    // define and initialize each board
    for (int i = 0; i < n_boards; i++){
        boards[i] = Adafruit_PWMServoDriver(i + PWM_SERVO_DEFAULT_ADDRESS);
        boards[i].begin(); 
        boards[i].setPWMFreq(freq);  // Analog servos run at ~60 Hz updates
    }
}

void ServoDriver::control(const int pin, const float angle){
    boards[pin/16].setPWM(pin%16, 0, angleToPulse(angle));
}