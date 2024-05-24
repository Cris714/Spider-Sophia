#include "leg.h"

class Spider{
    private:
        Leg legs[6];

    public:
        Spider(const int pins_config[6][3], const float legs_home[6][3]);
        void move_forward();
        void home_position();
};

Spider::Spider(const int pins_config[6][3], const float legs_home[6][3]){
    // initialize legs
    for (int i = 0; i < 6; i++)
        legs[i] = Leg(pins_config[i][0], pins_config[i][1], pins_config[i][2],
                        legs_home[i][0], legs_home[i][1], legs_home[i][2]);
}

void Spider::move_forward(){
    // moves forward only one step
    
}

void Spider::home_position(){
    // move all legs to its home position
    for (Leg leg: legs){
        leg.home();
        delay(100);
    }
}
