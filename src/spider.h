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
        void turn(bool direction);
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

    coords5f1[0] = new float[3]{9, -7, 4};
    coords5f1[1] = new float[3]{9, -4, 1.5};
    coords5f1[2] = new float[3]{9, -7, -3};

    coords5f2[0] = new float[3]{9, -7, 3};
    coords5f2[1] = new float[3]{9, -4, -1.5};
    coords5f2[2] = new float[3]{9, -7 , -3};

    coords5f3[0] = new float[3]{9, -7, 6};
    coords5f3[1] = new float[3]{9, -4, 1.5};
    coords5f3[2] = new float[3]{9, -7, -3};

    // coords5f1[0] = new float[3]{12, -6, 0};
    // coords5f1[1] = new float[3]{9, -6, -2.5};
    // coords5f1[2] = new float[3]{6, -6, -5};

    // coords5f2[0] = new float[3]{9, -3, 0};
    // coords5f2[1] = new float[3]{9, -7, 0};
    // coords5f2[2] = new float[3]{9, -9 , 0};

    // coords5f3[0] = new float[3]{0, -7, 0};
    // coords5f3[1] = new float[3]{4.5, -7, 0};
    // coords5f3[2] = new float[3]{9, -7, 0};


    auto points = trajectory_1s3(
        new int[3]{0, 1, 2}, 
        coords5f1,
        coords5f2,
        coords5f3,
        0.8
        );

    for (int i = 0; i < points.getSize(); i++){
        auto seq = points[i];
        auto seq2 = points[(i+points.getSize()/2)%points.getSize()];//points[(points.getSize()-i-1)%points.getSize()];
        legs[seq.p1.leg].state_update(leg_inverse_kinematics(seq.p1.x, seq.p1.y, seq.p1.z));
        legs[seq.p2.leg].state_update(leg_inverse_kinematics(seq.p2.x, seq.p2.y, seq.p2.z));
        legs[seq.p3.leg].state_update(leg_inverse_kinematics(seq.p3.x, seq.p3.y, seq.p3.z));
        legs[5-seq2.p3.leg].state_update(leg_inverse_kinematics(seq2.p3.x, seq2.p3.y, -seq2.p3.z));
        legs[5-seq2.p2.leg].state_update(leg_inverse_kinematics(seq2.p2.x, seq2.p2.y, -seq2.p2.z));
        legs[5-seq2.p1.leg].state_update(leg_inverse_kinematics(seq2.p1.x, seq2.p1.y, -seq2.p1.z));
        delay(12);
    }
    


    // // CALIBRATION ONLY -> {[-45 -> 45], [0 -> 90], [-90 -> 0]}
    // legs[5].state_update(leg_state({0, 0*90, -90}));
    // delay(2000);
    // legs[5].state_update(leg_state({0, 0, 0}));
    // delay(2000);
}

void Spider::turn(bool direction){ //true -> right, false -> left
    // moves forward only one step

    int n_points = 3;
    float *leg_init = new float[2]{0,1};

    float **coords = new float*[n_points];

    coords[0] = new float[3]{7, -9, 5.5};
    coords[1] = new float[3]{10.5, -4, 1.25};
    coords[2] = new float[3]{7, -9, -3};

    if(!direction) {
        float *temp = coords[0];
        coords[0] = coords[2];
        coords[2] = temp;
        leg_init = new float[2]{1,0};
    }

    int n_samples;
    float **mov = trajectory_line_multipoint(n_samples, coords, 1, n_points, false);

    for(int j=0; j<2; j++){
        for (int i = 0; i < n_samples; i++){
            // Avanza patas (j) (j+2) (j+4)
            for(int k=leg_init[j]; k<6; k += 2) {
                legs[k].state_update(leg_inverse_kinematics(mov[i][0], mov[i][1], mov[i][2]));
            }
            // Retrocede patas (1-j) (3-j) (5-j)
            for(int k=(1-leg_init[j]); k<6; k += 2) {
                legs[k].state_update(leg_inverse_kinematics(coords[0][0], coords[0][1], mov[n_samples -i -1][2]));
            }
            delay(12);
            
        }
    }
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
    }
}
