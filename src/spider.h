#include "leg.h"
#include "motion.h"

class Spider{
    private:
        Leg legs[6];

    public:
        Spider(const int pins_config[6][3], const float legs_home[6][3], 
            const int servo_min_pulses[6][3], const int servo_max_pulses[6][3]);
        void initialize();
        void move_forward();
        void home_position();
        void standup();
};

Spider::Spider(const int pins_config[6][3], const float legs_home[6][3], 
    const int servo_min_pulses[6][3], const int servo_max_pulses[6][3]){
    // initialize legs
    for (int i = 0; i < 6; i++)
        legs[i] = Leg(pins_config[i], legs_home[i], servo_min_pulses[i], servo_max_pulses[i]);
}

void Spider::initialize(){
    Leg{}.initialize();
}
 

void Spider::move_forward(){
    // moves forward only one step

    float **coords5f1 = new float*[3];
    float **coords5f2 = new float*[3];
    float **coords5f3 = new float*[3];

    coords5f1[0] = new float[3]{7, -7, 5.5};
    coords5f1[1] = new float[3]{10.5, -4, 1.25};
    coords5f1[2] = new float[3]{10, -7, -3};

    coords5f2[0] = new float[3]{9, -7, 1};
    coords5f2[1] = new float[3]{10.5, -4, -2.5};
    coords5f2[2] = new float[3]{12, -7 , -6};

    coords5f3[0] = new float[3]{12.5, -7, 5};
    coords5f3[1] = new float[3]{9.75, -4, -0.5};
    coords5f3[2] = new float[3]{7, -7, -6};


    auto points = trajectory_1s3(
        new int[3]{0, 1, 2}, 
        coords5f1,
        coords5f2,
        coords5f3,
        0.2
        );

    for (int i = 0; i < points.getSize(); i++){
        auto seq = points[i];
        for (int j = 0; j < seq.getSize(); j++){
            auto p = seq[j];
            legs[p.leg].state_update(leg_inverse_kinematics(p.x, p.y, p.z));
            delay(12);
        }
    }
    


    // // CALIBRATION ONLY
    // legs[2].state_update(leg_state({-45, 0, 0}));
    // delay(2000);
    // legs[2].state_update(leg_state({45, 0, 0}));
    // delay(2000);
}

void Spider::home_position(){
    // move all legs to its home position
    for (Leg leg: legs){
        leg.home();
        delay(10);
    }
}

void Spider::standup(){
    while(true){
        for (int i = 0; i < 6; i+=2){
            legs[i].state_update(leg_state(0.0, 28.5, -109.5));
        }
        
        delay(2000);
        for (int i = 0; i < 6; i+=2){
            legs[i].state_update(leg_state(0.0, 11.0, -95.2));
        }
        delay(2000);
    }
}
