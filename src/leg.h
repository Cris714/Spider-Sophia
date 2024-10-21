#include <math.h>
#include "servo_control.h"
#include <ArduinoEigen.h>

using Eigen::MatrixXf;

#define NTOF(x) (ntohs(x) - 32768) / 100.

struct t_point {
    float x;
    float y;
    float z;

    t_point(const float x, const float y, const float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    t_point() : x(0.0f), y(0.0f), z(0.0f) {}
};

struct t_point6{
    t_point p0;
    t_point p1;
    t_point p2; 
    t_point p3;
    t_point p4;
    t_point p5; 

    t_point6(t_point p0, t_point p1, t_point p2, t_point p3, t_point p4, t_point p5){
        this->p0 = p0;
        this->p1 = p1;
        this->p2 = p2;
        this->p3 = p3;
        this->p4 = p4;
        this->p5 = p5;
    }

    t_point6(MatrixXf points) {
        this->p0 = t_point( points(0, 0), points(0, 1), points(0,2) );
        this->p1 = t_point( points(1, 0), points(1, 1), points(1,2) );
        this->p2 = t_point( points(2, 0), points(2, 1), points(2,2) );
        this->p3 = t_point( points(3, 0), points(3, 1), points(3,2) );
        this->p4 = t_point( points(4, 0), points(4, 1), points(4,2) );
        this->p5 = t_point( points(5, 0), points(5, 1), points(5,2) );
    }
};

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

    leg_state(const float angles[NUM_SERVOS_PER_LEG]){
        hip_state = angles[0];
        femur_state = angles[1];
        tibia_state = angles[2];
    }
};

class Leg {
    private:
        static ServoDriver driver;

        void apply_transform(leg_state &state); 

        int pins[3];
        int min_pulses[3];
        int max_pulses[3];
        leg_state state;
        leg_state home_state;

    public: 
        static void initialize();
        // static leg_state leg_inverse_kinematics(const float x, const float y, const float z);

        Leg();
        Leg(const int pins[3], const float leg_home[3], const int min_pulses[3], const int max_pulses[3]);
        bool state_update(leg_state new_state);
        void home();
};

ServoDriver Leg::driver{ServoDriver(50)};

void Leg::initialize(){
    driver.initialize();
}

leg_state leg_inverse_kinematics(const float x, const float y, const float z){
    // leg's inverse kinematics, where x, y and z are the end-effector position wrt coxa's joint 
    // and NOT the spider center of mass 
    float theta0, theta1, theta2, xfemur, r, r2, x_intersect, dr;

    dr = sqrtf(x*x + z*z) - x;
    xfemur = x - HIP_LENGTH;
    // r2 = xfemur*xfemur + y*y + dr;
    r = sqrtf((xfemur + dr)*(xfemur + dr) + y*y);
    r2 = r*r;
    x_intersect = (r2 - TIBIA_LENGTH*TIBIA_LENGTH)/(2*FEMUR_LENGTH) + FEMUR_LENGTH/2;

    theta0 = atan2f(z, x);
    theta1 = atan2f(y, xfemur + dr) + acosf(x_intersect/sqrtf(r2));
    theta2 = -acosf((x_intersect - FEMUR_LENGTH)/TIBIA_LENGTH);

    // Serial.printf("ANGLES: %f, %f, %f\n", RAD_TO_DEG*theta0, RAD_TO_DEG*theta1, RAD_TO_DEG*theta2);

    return leg_state(RAD_TO_DEG*theta0, RAD_TO_DEG*theta1, RAD_TO_DEG*theta2);
}

// method definitions
Leg::Leg(){
    /// dummy default constructor

}

Leg::Leg(const int pins[3], const float leg_home[3], const int min_pulses[3], const int max_pulses[3]){
    // initialize leg control pins and max pulses
    
    for (int i = 0; i < 3; i++){
        this->pins[i] = pins[i];
        this->min_pulses[i] = min_pulses[i];
        this->max_pulses[i] = max_pulses[i];
    }

    // set home_state
    home_state = leg_state(leg_home);
}

void Leg::apply_transform(leg_state &state_){
    state_.hip_state += home_state.hip_state;
    state_.femur_state += home_state.femur_state;
    state_.tibia_state -= home_state.tibia_state;
}

bool Leg::state_update(leg_state new_state){
    // apply_transform(new_state);
    new_state.hip_state += home_state.hip_state;
    new_state.femur_state += home_state.femur_state;
    new_state.tibia_state = -new_state.tibia_state + home_state.tibia_state;

    // Serial.printf("%f, %f, %f\n", new_state.hip_state, new_state.femur_state, new_state.tibia_state);

    // check new state satisfies min angles
    if (new_state.hip_state < MIN_HIP_ANGLE || 
                new_state.femur_state < MIN_FEMUR_ANGLE || 
                new_state.tibia_state < MIN_TIBIA_ANGLE )
        return false;

    // check new state satisfies max angles
    if (new_state.hip_state > MAX_HIP_ANGLE || 
                new_state.femur_state > MAX_FEMUR_ANGLE || 
                new_state.tibia_state > MAX_TIBIA_ANGLE)
        return false;

    // set new state
    driver.control(pins[0], new_state.hip_state, min_pulses[0], max_pulses[0]); // hip
    driver.control(pins[1], new_state.femur_state, min_pulses[1], max_pulses[1]); // femur
    driver.control(pins[2], new_state.tibia_state, min_pulses[2], max_pulses[2]); // tibia

    state = new_state;

    return true;
}

void Leg::home(){
    // set home state
    driver.control(pins[0], home_state.hip_state, min_pulses[0], max_pulses[0]); // hip
    driver.control(pins[1], home_state.femur_state, min_pulses[1], max_pulses[1]); // femur
    driver.control(pins[2], home_state.tibia_state, min_pulses[2], max_pulses[2]); // tibia

    state = home_state;
}