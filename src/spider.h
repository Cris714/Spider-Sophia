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
 

void Spider::turn(bool direction){ //true -> right, false -> left
    // moves forward only one step

    int n_points = 3;

    float **coords = new float*[n_points];

    coords[0] = new float[3]{7, -9, 5.5};
    coords[1] = new float[3]{10.5, -4, 1.25};
    coords[2] = new float[3]{10, -9, -3};

    if(!direction) {
        float *temp = coords[0];
        coords[0] = coords[2];
        coords[2] = temp;
    }

    int n_samples;
    float **mov = trajectory_line_multipoint(n_samples, coords, 1, n_points, false);

    for(int j=0; j<2; j++){
        for (int i = 0; i < n_samples; i++){
            // Avanza patas (j) (j+2) (j+4)
            for(int k=j; k<6; k += 2) {
                legs[k].state_update(leg_inverse_kinematics(mov[i][0], mov[i][1], mov[i][2]));
            }
            // Retrocede patas (1-j) (3-j) (5-j)
            for(int k=(1-j); k<6; k += 2) {
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
        
        delay(2000);
        for (int i = 0; i < 6; i+=2){
            legs[i].state_update(leg_state(0.0, 11.0, -95.2));
        }
        delay(2000);
    }
}
