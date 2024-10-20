#include "leg.h"
#include "motion.h"

class Spider{
    private:
        Leg legs[NUM_LEGS];

    public:
        Spider(const int pins_config[NUM_LEGS][NUM_SERVOS_PER_LEG], const float legs_home[NUM_LEGS][NUM_SERVOS_PER_LEG], 
            const int servo_min_pulses[NUM_LEGS][NUM_SERVOS_PER_LEG], const int servo_max_pulses[NUM_LEGS][NUM_SERVOS_PER_LEG]);
        void initialize();
        void home_position();
        void standup();
        void set_coords(const t_point6& coord_state);
        void set_state(const t_point6& state);
};

Spider::Spider(const int pins_config[NUM_LEGS][NUM_SERVOS_PER_LEG], const float legs_home[NUM_LEGS][NUM_SERVOS_PER_LEG], 
    const int servo_min_pulses[NUM_LEGS][NUM_SERVOS_PER_LEG], const int servo_max_pulses[NUM_LEGS][NUM_SERVOS_PER_LEG]){
    // initialize legs
    for (int i = 0; i < NUM_LEGS; i++)
        legs[i] = Leg(pins_config[i], legs_home[i], servo_min_pulses[i], servo_max_pulses[i]);
}

void Spider::initialize(){
    Leg{}.initialize();
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
        for (int i = 0; i < NUM_LEGS; i+=2){
            legs[i].state_update(leg_state(0.0, 28.5, -109.5));
        }
    }
}

void Spider::set_coords(const t_point6& coord_state){
    legs[0].state_update(leg_inverse_kinematics(coord_state.p0.x, coord_state.p0.y, coord_state.p0.z));
    legs[1].state_update(leg_inverse_kinematics(coord_state.p1.x, coord_state.p1.y, coord_state.p1.z));
    legs[2].state_update(leg_inverse_kinematics(coord_state.p2.x, coord_state.p2.y, coord_state.p2.z));
    legs[3].state_update(leg_inverse_kinematics(coord_state.p3.x, coord_state.p3.y, coord_state.p3.z));
    legs[4].state_update(leg_inverse_kinematics(coord_state.p4.x, coord_state.p4.y, coord_state.p4.z));
    legs[5].state_update(leg_inverse_kinematics(coord_state.p5.x, coord_state.p5.y, coord_state.p5.z));
}

void Spider::set_state(const t_point6& state){
    legs[0].state_update({state.p0.x, state.p0.y, state.p0.z});
    legs[1].state_update({state.p1.x, state.p1.y, state.p1.z});
    legs[2].state_update({state.p2.x, state.p2.y, state.p2.z});
    legs[3].state_update({state.p3.x, state.p3.y, state.p3.z});
    legs[4].state_update({state.p4.x, state.p4.y, state.p4.z});
    legs[5].state_update({state.p5.x, state.p5.y, state.p5.z});
}