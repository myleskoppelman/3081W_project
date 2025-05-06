#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <vector>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "physics.h"
#include "globals.h"

class Object {
public:
    float r; 
    long double m;
    std::vector<double> pos;
    std::vector<double> vel;
    float segments = 50.0f;

    Object() : r(0.0f), m(0.0), pos({0.0, 0.0}), vel({0.0, 0.0}) {}

    Object(float radius, long double mass, const std::vector<double>& position, const std::vector<double>& velocity); // velocity = km/s

    void draw() const;
    void acc(long double x, long double y);
    void move();
    void set_vel(float x, float y);
    std::vector<double> get_coords() const;
    bool check_boundry();
    bool check_collision(std::vector<Object>& objs);
};

#endif // OBJECT_H
