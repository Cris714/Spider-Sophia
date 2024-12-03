#include <algorithm>

using namespace std;
using Eigen::Matrix4f;

#define LMT(coord) min(max(coord, -256.0f), 256.0f)

class BodyFrameControl {
    private:
        Matrix4f T_SB;
        
        Matrix4f T_BMs[6];

        Matrix4f T_MCs_i[6];
        Matrix4f T_MCs_f[6];

        Matrix4f T_SCs[6];

    public:
        BodyFrameControl();
        t_point6 send_multiple(vector<vector<float>>& moves, vector<vector<int>>& targets, const int n);
        Matrix4f T(const vector<float>& crds);
};

BodyFrameControl::BodyFrameControl() {
    // initial body frame wrt space frame 
    this->T_SB = T({0, 0, 0, 0, 0, 0});

    // leg-origin frame wrt body frame
    this->T_BMs[0] = T({ 3.025, 0,  5.25, 0,  radians(-60), 0});
    this->T_BMs[1] = T({  6.05, 0,     0, 0,             0, 0});
    this->T_BMs[2] = T({ 3.025, 0, -5.25, 0, radians(-300), 0});
    this->T_BMs[3] = T({-3.025, 0, -5.25, 0, radians(-240), 0});
    this->T_BMs[4] = T({ -6.05, 0,     0, 0, radians(-180), 0});
    this->T_BMs[5] = T({-3.025, 0,  5.25, 0, radians(-120), 0});

    // end-effector frame wrt leg-origin frame
    for(int i=0; i<6; i++) {
        this->T_MCs_i[i] = T({7, -7, 0, 0, 0, 0});
        this->T_MCs_f[i] = T({7, -7, 0, 0, 0, 0});
    }

    // initial end-effector frame wrt space frame 
    for(int i=0; i<6; i++) this->T_SCs[i] = this->T_SB*this->T_BMs[i]*this->T_MCs_i[i];
}

t_point6 BodyFrameControl::send_multiple(vector<vector<float>>& moves, vector<vector<int>>& targets, const int n) {
    Matrix4f T_SMs_inv[6];
    Matrix4f T_SB_f;
    vector<int> moved_legs(6, 0);
    vector<float> move;
    vector<int> target;

    for(int i=0; i<n; i++) {
        move = moves[i];
        target = targets[i];
        T_SB_f = T(move);
        for(int j=0; j<6; j++) {
            if(target[j]) {
                try {
                    if(moved_legs[j]) runtime_error("Sobrelapamiento de movimientos");
                    moved_legs[j] = 1;
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

Matrix4f BodyFrameControl::T(const vector<float>& crds){
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