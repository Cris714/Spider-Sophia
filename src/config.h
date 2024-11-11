#define NUM_LEGS 6
#define NUM_SERVOS_PER_LEG 3
#define MAX_INTERVAL 1 // Intervalo de espera (s)

// wifi_ap
#define SID "SPINARAK32"
#define PSWD "qwer1234"

// wifi_config
// #define SID "Wifi_14000"
// #define PSWD "wifi14000"
#define PORT 3000

// body_frame_control
#define DELAY 0.025
#define H -2

// leg
#define MIN_HIP_ANGLE 0
#define MIN_FEMUR_ANGLE 0
#define MIN_TIBIA_ANGLE -50

#define MAX_HIP_ANGLE 225
#define MAX_FEMUR_ANGLE 225
#define MAX_TIBIA_ANGLE 225

#define HIP_LENGTH 3.5
#define FEMUR_LENGTH 4.5
#define TIBIA_LENGTH 9

const int servo_input_pins[NUM_LEGS][NUM_SERVOS_PER_LEG] = { 
    { 16, 17, 18 }, // pata 1
    { 20, 21, 22 }, // pata 2
    { 24, 25, 26 }, // pata 3
    {  0,  1,  2 }, // pata 4
    {  4,  5,  6 }, // pata 5
    {  8,  9, 10 }  // pata 6 
};

const float servo_home_state_angles[NUM_LEGS][NUM_SERVOS_PER_LEG] = {
    { 130, 33, 0 },
    { 107, 55, 15 },
    { 130, 50, 20 },
    { 130, 56, 15 },
    { 55, 35, 0 },
    { 105, 48, 7 }
};

const int servo_min_pulse[NUM_LEGS][NUM_SERVOS_PER_LEG] = {
    { 140, 140, 140 },
    { 140, 140, 140 },
    { 140, 140, 140 },
    { 140, 140, 140 },
    { 140, 140, 140 },
    { 140, 140, 140 }
};

const int servo_max_pulse[NUM_LEGS][NUM_SERVOS_PER_LEG] = {
    { 560, 560, 560 },
    { 560, 560, 560 },
    { 560, 560, 560 },
    { 560, 560, 560 },
    { 560, 560, 560 },
    { 560, 560, 560 }
};