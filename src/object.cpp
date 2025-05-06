#include <iostream>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "object.h"
#include "physics.h"


long double dt = delta_t;



Object::Object(float radius, long double mass, const std::vector<double>& position, const std::vector<double>& velocity)
    : r(radius), m(mass), pos(position), vel(velocity) {}

void Object::draw() const {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2d(pos[0], pos[1]);
    for (unsigned int i = 0; i <= segments; ++i) {
        float angle = (2.0f * M_PI) * (static_cast<float>(i) / segments);
        float px = pos[0] + r * cos(angle) ;
        float py = pos[1] + r * sin(angle);
        glVertex2d(px, py);
    }
    glEnd();
}

void Object::acc(long double x, long double y) {
    vel[0] += x ;
    vel[1] += y ;
    // std::cout.precision(10); // Set precision to 10 decimal places
    // std::cout << std::fixed;
    // std::cout << "Acceleration x: " << x * dt << std::endl;
    // std::cout << "Acceleration y: " << y * dt << std::endl;
    // std::cout << "x: " << x << std::endl;
    // std::cout << "y: " << y << std::endl;
    // std::cout << "dt: " << delta_t << std::endl;
}

void Object::move() {
    pos[0] += vel[0];
    pos[1] += vel[1];
}

void Object::set_vel(float x, float y) {
    vel[0] *= x;
    vel[1] *= y;
}

std::vector<double> Object::get_coords() const {
    return pos;
}

bool Object::check_boundry() {
    if (pos[1] > 1.0f || pos[1] < -1.0f) {
        vel[1] *= -1.0f;
        return true;
    }
    if (pos[0] > 1.0f || pos[0] < -1.0f) {
        vel[0] *= -1.0f;
        return true;
    }
    return false;
}

bool Object::check_collision(std::vector<Object>& objs) {
    for (auto& obj : objs) {
        if (&obj == this) continue;
        float dx = obj.get_coords()[0] - pos[0];
        float dy = obj.get_coords()[1] - pos[1];
        float distance = sqrt(dx * dx + dy * dy);
        if (distance < (r + obj.r)) {
            obj.set_vel(-1.0f, -1.0f);
            return true;
        }
    }
    return false;
}
    

