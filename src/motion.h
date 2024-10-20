#include <math.h>
#include "linkedlist.h"
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

float **trajectory_line_multipoint(int &n_samples, float **points, const float dist, const int n_points, const bool close){
    int  count;
    float a, b, c, D, gamma;

    int it = n_points;
    if(close) {
        it++;  
    }

    n_samples = it - 1;
    for (int i = 0; i < it - 1; i++){
        a = points[(i+1)%n_points][0] - points[i][0];
        b = points[(i+1)%n_points][1] - points[i][1];
        c = points[(i+1)%n_points][2] - points[i][2];
        n_samples += (int)floorf(sqrtf(a*a + b*b + c*c)/dist);
    }

    float **samples = new float*[n_samples];

    count = 0;
    for (int i = 0; i < n_points - 1; i++){
        a = points[(i+1)%n_points][0] - points[i][0];
        b = points[(i+1)%n_points][1] - points[i][1];
        c = points[(i+1)%n_points][2] - points[i][2];
        D = sqrtf(a*a + b*b + c*c);
        gamma = dist/D;
        for (int j = 0; j < (int)(D/dist); j++){
            samples[count++] = new float[3]{points[i][0] + j*gamma*a, points[i][1] + j*gamma*b, points[i][2] + j*gamma*c};
        }
        samples[count++] = new float[3]{points[(i+1)%n_points][0], points[(i+1)%n_points][1], points[(i+1)%n_points][2]};
    }

    return samples;
}