#include <iostream>
#include <vector>
#include <cmath>
#include "globals.h"

long double delta_t = 1;
float G = 6.67 * pow(10, -11);        


long double calculate_g_force(long double mass1, long double mass2, double distance){
    return (G * mass1 * mass2) / pow(distance, 2); // kg m/s^2
}

long double calculate_distance(double dx, double dy){
    return sqrt(pow(dx, 2) + pow(dy, 2));
}

std::vector<double> calculate_direction(double dx, double dy, double distance) {
    std::vector<double> direction = {(dx / distance), (dy / distance)};
    return direction; // ms; 
}
