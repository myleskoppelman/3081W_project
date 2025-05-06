#ifndef PHYSICS_H
#define PHYSICS_H

#include <iostream>
#include <vector>
#include <cmath>
#include "physics.h"


long double calculate_g_force(long double mass1, long double mass2, double distance);
long double calculate_distance(double dx, double dy);
std::vector<double> calculate_direction(double dx, double dy, double distance);

#endif