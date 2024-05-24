#include "servo_control.h"

#define MIN_HIP_ANGLE 0
#define MIN_FEMUR_ANGLE 0
#define MIN_TIBIA_ANGLE 0

#define MAX_HIP_ANGLE 180
#define MAX_FEMUR_ANGLE 180
#define MAX_TIBIA_ANGLE 180



struct leg_state
{
    float hip_state;
    float femur_state;
    float tibia_state;

    leg_state(){
        hip_state = 0.f;
        femur_state = 0.f;
        tibia_state = 0.f;
    }

    leg_state(const float hip_angle, const float femur_angle, const float tibia_angle){
        hip_state = hip_angle;
        femur_state = femur_angle;
        tibia_state = tibia_angle;
    }
};

class Leg {
    private:
        static ServoDriver driver;
        int pins[3];
        leg_state state;
        leg_state home_state;

    public: 
        Leg();
        Leg(const int pin_hip, const int pin_femur, const int pin_tibia, 
            const float hip_home, const float femur_home, const float tibia_home);
        bool state_update(leg_state new_state);
        void home();
};

ServoDriver Leg::driver{ServoDriver(2, 60)};

// method definitions
Leg::Leg(){
    /// dummy default constructor

}

Leg::Leg(const int pin_hip, const int pin_femur, const int pin_tibia, 
            const float hip_home, const float femur_home, const float tibia_home){
    // initialize leg control pins
    pins[0] = pin_hip;
    pins[1] = pin_femur;
    pins[2] = pin_tibia;

    // set home_state
    home_state = leg_state(hip_home, femur_home, tibia_home);
}

bool Leg::state_update(leg_state new_state){
    // check new state satisfies min angles
    if (new_state.hip_state < MIN_HIP_ANGLE || 
                new_state.femur_state < MIN_FEMUR_ANGLE || 
                new_state.tibia_state < MIN_TIBIA_ANGLE )
        return false;

    // check new state satisfies max angles
    if (new_state.hip_state > MAX_HIP_ANGLE || 
                new_state.femur_state < MAX_FEMUR_ANGLE || 
                new_state.tibia_state < MAX_TIBIA_ANGLE)
        return false;

    // set new state
    driver.control(pins[0], new_state.hip_state); // hip
    driver.control(pins[1], new_state.femur_state); // femur
    driver.control(pins[2], new_state.tibia_state); // tibia

    state = new_state;

    return true;
}

void Leg::home(){
    // set home state
    driver.control(pins[0], home_state.hip_state); // hip
    driver.control(pins[1], home_state.femur_state); // femur
    driver.control(pins[2], home_state.tibia_state); // tibia

    state = home_state;
}