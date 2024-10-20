#include <ArduinoEigen.h>
#include <algorithm>

using namespace std;
using Eigen::Matrix4f;
using Eigen::MatrixXf;

class BodyFrameControl {
    private:
        int i;
        bool moving;
        float last_yaw;
        float last_i;

        Matrix4f T_SB;
        
        Matrix4f T_BMs[6];

        Matrix4f T_MCs_i[6];
        Matrix4f T_MCs_f[6];

        Matrix4f T_SCs[6];

    public:
        BodyFrameControl();
        t_point6 moveBodyFrame(float** moves, int** targets, const int n);
        void move_around(Spider &spider, const float yaw, const float I);
        void finishMovement(Spider &spider);
        Matrix4f T(const float* crds);
        bool isMoving();
};

BodyFrameControl::BodyFrameControl() {
    this->i = 0;
    this->moving = false;
    // initial body frame wrt space frame 
    this->T_SB = T(new float[6]{0, 0, 0, 0, 0, 0});

    // leg-origin frame wrt body frame
    this->T_BMs[0] = T(new float[6]{ 2.5, 0,  4.625, 0,  radians(-55), 0});
    this->T_BMs[1] = T(new float[6]{   4, 0,      0, 0,             0, 0});
    this->T_BMs[2] = T(new float[6]{ 2.5, 0, -4.625, 0, radians(-300), 0});
    this->T_BMs[3] = T(new float[6]{-2.5, 0, -4.625, 0, radians(-245), 0});
    this->T_BMs[4] = T(new float[6]{  -4, 0,      0, 0, radians(-180), 0});
    this->T_BMs[5] = T(new float[6]{-2.5, 0,  4.625, 0, radians(-123), 0});

    // end-effector frame wrt leg-origin frame
    for(int i=0; i<6; i++) {
        this->T_MCs_i[i] = T(new float[6]{7, -7, 0, 0, 0, 0});
        this->T_MCs_f[i] = T(new float[6]{7, -7, 0, 0, 0, 0});
    }

    // initial end-effector frame wrt space frame 
    for(int i=0; i<6; i++) this->T_SCs[i] = this->T_SB*this->T_BMs[i]*this->T_MCs_i[i];
}

t_point6 BodyFrameControl::moveBodyFrame(float** moves, int** targets, const int n) {
    Matrix4f T_SMs_inv[6];
    Matrix4f T_SB_f;
    bool* moved_legs = new bool[6]{false, false, false, false, false};
    float* move;
    int* target;

    for(int i=0; i<n; i++) {
        move = moves[i];
        target = targets[i];
        T_SB_f = T(move);
        for(int j=0; j<6; j++) {
            if(target[j]) {
                try {
                    if(moved_legs[j]) runtime_error("Sobrelapamiento de movimientos");
                    moved_legs[j] = true;
                    T_SMs_inv[j] = (T_SB_f * this->T_BMs[j]).inverse();
                    this->T_MCs_f[j] = T_SMs_inv[j] * this->T_SCs[j];

                } catch (const std::exception& e) {
                    Serial.printf("Excepción capturada: %s\n", e.what());
                }
            }
        }
    }

    MatrixXf points(NUM_LEGS, NUM_SERVOS_PER_LEG);

    for(int i=0; i<NUM_LEGS; i++) {
        // copy final positions of each leg
        for(int row = 0; row < NUM_SERVOS_PER_LEG; row++) {
            points(i, row) = LMT( this->T_MCs_f[i](row, this->T_MCs_f[i].cols()-1) );
        }
    }

    return t_point6( points );

}

void BodyFrameControl::move_around(Spider &spider, const float yaw, const float I) {
    float A = I + 2.8;
    float dx = A * sinf(yaw);               // parte x vector de avance
    float dz = A * cosf(yaw);               // parte z vector de avance
    float I_alpha = PI * I / (2 * A - I);   // intervalo de muestreo semielipse

    int t = 0;
    float alpha;
    float** moves = new float*[2];
    int** targets = new int*[2];

    // se recibe información de movimiento por primera vez
    if(!(this->moving)) {
        for(float amp=0; amp<A; amp += I/2) {
            alpha = PI - I_alpha * t;
            moves[0] = new float[6]{    amp * sinf(yaw),               0,     amp * cosf(yaw), 0, 0, 0};
            moves[1] = new float[6]{-dx * cosf(alpha/2), H * sinf(alpha), -dz * cosf(alpha/2), 0, 0, 0};
            targets[0] = new int[6]{0, 1, 0, 1, 0, 1};
            targets[1] = new int[6]{1, 0, 1, 0, 1, 0};

            spider.set_coords( moveBodyFrame(moves, targets, 2) );

            t++;
            delay(DELAY*1000);
        }
        this->moving = true;
    }

    if(this->moving) {
        // se mantiene la información de movimient
        t = 0;
        for(float amp=-A; amp<A; amp += I) {
            alpha = I_alpha * t;
            moves[0] = new float[6]{ amp * sinf(yaw),               0,  amp * cosf(yaw), 0, 0, 0};
            moves[1] = new float[6]{dx * cosf(alpha), H * sinf(alpha), dz * cosf(alpha), 0, 0, 0};
            targets[0] = new int[6]{~i&1, i&1, ~i&1, i&1, ~i&1, i&1};
            targets[1] = new int[6]{i&1, ~i&1, i&1, ~i&1, i&1, ~i&1};

            spider.set_coords( moveBodyFrame(moves, targets, 2) );

            t++;
            delay(DELAY*1000);
        }

        this->i++;
        this->last_yaw = yaw;
        this->last_i = I;
    }
}

void BodyFrameControl::finishMovement(Spider &spider) {
    float A = this->last_i + 2.8;
    Serial.print("Finish movement...\n");
    float dx = A * sinf(this->last_yaw);               // parte x vector de avance
    float dz = A * cosf(this->last_yaw);               // parte z vector de avance
    float I_alpha = PI * this->last_i / (2 * A - this->last_i);   // intervalo de muestreo semielipse

    int t = 0;
    float alpha;
    float** moves = new float*[2];
    int** targets = new int*[2];

    // si ya no se recibe información de movimiento
    for(float amp=-A; amp<0; amp += (this->last_i)/2) {
        alpha = I_alpha * t;
        moves[0] = new float[6]{amp * sinf(this->last_yaw),               0, amp * cosf(this->last_yaw), 0, 0, 0};
        moves[1] = new float[6]{        dx * cosf(alpha/2), H * sinf(alpha),         dz * cosf(alpha/2), 0, 0, 0};
        targets[0] = new int[6]{~i&1, i&1, ~i&1, i&1, ~i&1, i&1};
        targets[1] = new int[6]{i&1, ~i&1, i&1, ~i&1, i&1, ~i&1};

        t_point6 next_point = moveBodyFrame(moves, targets, 2);

        spider.set_coords( next_point );

        t++;
        delay(DELAY*1000);
    }

    this->i = 0;
    this->moving = false;
}

Matrix4f BodyFrameControl::T(const float* crds){
    Matrix4f m;

    float x = crds[0];
    float y = crds[1];
    float z = crds[2];
    float theta = crds[3];
    float phi = crds[4];
    float psi = crds[5];

    m << cos(phi)*cos(psi), -cos(phi)*sin(psi), sin(phi), x,
        sin(theta)*sin(phi)*cos(psi)+cos(theta)*sin(psi), -sin(theta)*sin(phi)*sin(psi)+cos(theta)*cos(psi), -sin(theta)*cos(phi), y,
        -cos(theta)*sin(phi)*cos(psi)+sin(theta)*sin(psi), cos(theta)*sin(phi)*sin(psi)+sin(theta)*cos(psi), cos(theta)*cos(phi), z,
        0., 0., 0., 1.;

    return m;
}

bool BodyFrameControl::isMoving() {
    return this->moving;
}