#include "Generator.h"
#include <Arduino.h>

Generator::Generator(/* args */)
{

}

void Generator::generateCurvature(double path[][ARRAY_SIZE]) {
    for(int i = 0; i < sizeof(path); i++) {
        double x1 = path[i-1][X_INDEX] + 0.001;
        double y1 = path[i-1][Y_INDEX];
        double x2 = path[i][X_INDEX];
        double y2 = path[i][Y_INDEX];
        double x3 = path[i+1][X_INDEX];
        double y3 = path[i+1][Y_INDEX];
        double k1 = 0.5 * (pow(x1,2) + pow(y1,2) - pow(x2,2) - pow(y2,2)) / (x1 - x2);
        double k2 = (y1-y2)/(x1-x2);
        double b = 0.5 * (pow(x2,2)-2*x2*k1+pow(y2,2)-pow(x3,2)+2*x3*k1-pow(y3,2))/(x3*k2-y3+y2-x2*k2);
        double a = k1-k2 * b;
        double r = sqrt(pow(x1-a,2)+pow(y1-b,2));
        double curvature = 1/r;
        
    }
}