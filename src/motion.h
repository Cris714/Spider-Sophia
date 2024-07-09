#include <math.h>
#include "linkedlist.h"

struct t_point{
    int leg;
    float x;
    float y;
    float z;

    t_point(const int leg, const float x, const float y, const float z){
        this->leg = leg;
        this->x = x;
        this->y = y;
        this->z = z;
    }
};

List<List<t_point>> trajectory_1s3(
        const int legs[3],
        float **l0t, 
        float **l1t, 
        float **l2t,
        const float dist
    ){
    float dx0, dy0, dz0, dx1, dy1, dz1, dx2, dy2, dz2, D, gamma;
    List<List<t_point>> points;

    // seq 1
    for (int i = 0; i < 3 - 1; i++){
        dx0 = l0t[i+1][0] - l0t[i][0];
        dy0 = l0t[i+1][1] - l0t[i][1];
        dz0 = l0t[i+1][2] - l0t[i][2];

        dx1 = (l1t[0][0] - l1t[2][0])/4;
        dz1 = (l1t[0][2] - l1t[2][2])/4;

        dx2 = (l2t[0][0] - l2t[2][0])/4;
        dz2 = (l2t[0][2] - l2t[2][2])/4;

        D = sqrtf(dx0*dx0 + dy0*dy0 + dz0*dz0);
        gamma = dist/D;
        for (int j = 0; j < (int)(D/dist); j++){
            List<t_point> sequence;
            sequence.add(t_point(legs[0], l0t[i][0] + j*gamma*dx0, l0t[i][1] + j*gamma*dy0, l0t[i][2] + j*gamma*dz0));
            sequence.add(t_point(legs[1], l1t[2][0] + j*gamma*dx1, l1t[0][1], l1t[2][2] + j*gamma*dz1));
            sequence.add(t_point(legs[2], (l2t[0][0] + l2t[2][0])/2 + j*gamma*dx2, l2t[0][1], (l2t[0][2] + l2t[2][2])/2 + j*gamma*dz2));
            points.add(sequence);
            Serial.println("PASO\n\n\n\n\n\n");
            delay(3000);
        }
    }

    // seq 2
    for (int i = 0; i < 3 - 1; i++){
        dx2 = l2t[i+1][0] - l2t[i][0];
        dy2 = l2t[i+1][1] - l2t[i][1];
        dz2 = l2t[i+1][2] - l2t[i][2];

        dx0 = (l0t[0][0] - l0t[2][0])/4;
        dz0 = (l0t[0][2] - l0t[2][2])/4;

        dx1 = (l1t[0][0] - l1t[2][0])/4;
        dz1 = (l1t[0][2] - l1t[2][2])/4;

        D = sqrtf(dx2*dx2 + dy2*dy2 + dz2*dz2);
        gamma = dist/D;
        for (int j = 0; j < (int)(D/dist); j++){
            List<t_point> sequence;
            sequence.add(t_point(legs[2], l2t[i][0] + j*gamma*dx2, l2t[i][1] + j*gamma*dy2, l2t[i][2] + j*gamma*dz2));
            sequence.add(t_point(legs[0], l0t[2][0] + j*gamma*dx0, l0t[0][1], l0t[2][2] + j*gamma*dz0));
            sequence.add(t_point(legs[1], (l1t[0][0] + l1t[2][0])/2 + j*gamma*dx1, l1t[0][1], (l1t[0][2] + l1t[2][2])/2 + j*gamma*dz1));
            points.add(sequence);
        }
    }

    // seq 3
    for (int i = 0; i < 3 - 1; i++){
        dx1 = l1t[i+1][0] - l1t[i][0];
        dy1 = l1t[i+1][1] - l1t[i][1];
        dz1 = l1t[i+1][2] - l1t[i][2];

        dx0 = (l0t[0][0] - l0t[2][0])/4;
        dz0 = (l0t[0][2] - l0t[2][2])/4;

        dx2 = (l2t[0][0] - l2t[2][0])/4;
        dz2 = (l2t[0][2] - l2t[2][2])/4;

        D = sqrtf(dx1*dx1 + dy1*dy1 + dz1*dz1);
        gamma = dist/D;
        for (int j = 0; j < (int)(D/dist); j++){
            List<t_point> sequence;
            sequence.add(t_point(legs[1], l1t[i][0] + j*gamma*dx1, l1t[i][1] + j*gamma*dy1, l1t[i][2] + j*gamma*dz1));
            sequence.add(t_point(legs[2], l2t[2][0] + j*gamma*dx2, l2t[0][1], l2t[2][2] + j*gamma*dz2));
            sequence.add(t_point(legs[0], (l0t[0][0] + l0t[2][0])/2 + j*gamma*dx0, l0t[0][1], (l0t[0][2] + l0t[2][2])/2 + j*gamma*dz0));
            points.add(sequence);
        }
    }

    return points;
    }

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