#include <ArduinoEigen.h>
#include <algorithm>

using namespace std;
using Eigen::Matrix4f;
using Eigen::MatrixXf;

#define LMT(coord) min(max(coord, -256.0f), 256.0f)

class BodyFrameControl {
    private:
        int TARGET_LEGS[6];

        Matrix4f T_SB;
        
        Matrix4f T_BMs[6];

        Matrix4f T_MCs_i[6];
        Matrix4f T_MCs_f[6];

        Matrix4f T_SCs[6];

    public:
        BodyFrameControl();
        MatrixXf moveBodyFrame(const float x, const float y, const float z, const float theta, const float phi, const float psi);
        Matrix4f T(const float x, const float y, const float z, const float theta, const float phi, const float psi);
        void printMat();
};

BodyFrameControl::BodyFrameControl() {
    // initial body frame wrt space frame 
    this->T_SB = T(0, 0, 0, 0, 0, 0);

    // leg-origin frame wrt body frame
    this->T_BMs[0] = T( 2.5, 0,  4.625, 0,  radians(-55), 0);
    this->T_BMs[1] = T(   4, 0,      0, 0,             0, 0);
    this->T_BMs[2] = T( 2.5, 0, -4.625, 0, radians(-300), 0);
    this->T_BMs[3] = T(-2.5, 0, -4.625, 0, radians(-245), 0);
    this->T_BMs[4] = T(  -4, 0,      0, 0, radians(-180), 0);
    this->T_BMs[5] = T(-2.5, 0,  4.625, 0, radians(-123), 0);

    // end-effector frame wrt leg-origin frame
    for(int i=0; i<6; i++) {
        this->T_MCs_i[i] = T(7, -7, 0, 0, 0, 0);
        this->T_MCs_f[i] = T(7, -7, 0, 0, 0, 0);
    }

    // initial end-effector frame wrt space frame 
    for(int i=0; i<6; i++) {
        this->T_SCs[i] = this->T_SB*this->T_BMs[i]*this->T_MCs_i[i];
        this->TARGET_LEGS[i] = 1;
    }
}

MatrixXf BodyFrameControl::moveBodyFrame(const float x, const float y, const float z, 
                                     const float theta, const float phi, const float psi) {
    Matrix4f T_SMs_inv[6];

    Matrix4f T_SB_f = T(x, y, z, theta, phi, psi);

    MatrixXf points(6,3);

    for(int i=0; i<6; i++) {
        if( this->TARGET_LEGS[i] ) {
            T_SMs_inv[i] = (T_SB_f * this->T_BMs[i]).inverse();
            this->T_MCs_f[i] = T_SMs_inv[i] * this->T_SCs[i];
        }

        // copy final positions of each leg
        for(int row = 0; row < 3; row++) {
            points(i, row) = LMT( this->T_MCs_f[i](row, this->T_MCs_f[i].cols()-1) );

            // Serial.print(points(i, row), 4);
            // Serial.print("\t");
        }
        // Serial.println();
    }

    return points;

}

Matrix4f BodyFrameControl::T(const float x, const float y, const float z, const float theta, const float phi, const float psi){
    Matrix4f m;

    m << cos(phi)*cos(psi), -cos(phi)*sin(psi), sin(phi), x,
        sin(theta)*sin(phi)*cos(psi)+cos(theta)*sin(psi), -sin(theta)*sin(phi)*sin(psi)+cos(theta)*cos(psi), -sin(theta)*cos(phi), y,
        -cos(theta)*sin(phi)*cos(psi)+sin(theta)*sin(psi), cos(theta)*sin(phi)*sin(psi)+sin(theta)*cos(psi), cos(theta)*cos(phi), z,
        0., 0., 0., 1.;

    return m;
}

void BodyFrameControl::printMat() {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            Serial.print(this->T_MCs_f[0](i, j), 4);
            Serial.print("\t");
        }
        Serial.println(); 
    }
}