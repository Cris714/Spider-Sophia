#include <math.h>

float **trajectory_line_multipoint(int &n_samples, float **points, const float dist, const int n_points, const bool close){
    int  count;
    float a, b, c, D, gamma;

    n_samples = n_points - 1;
    for (int i = 0; i < n_points - 1; i++){
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