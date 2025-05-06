#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>
#include <cmath>
#include <sstream>

#include "object.h"
#include "physics.h"
#include "startup.h"
#include "text_renderer.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
#include "globals.h"

extern TextRenderer text_renderer;
float pause_btn_x = -0.95f, reset_btn_x = -0.7f, speed_up_x = -0.2f, btn_w = 0.2f, btn_h = 0.13f,
speed_down_x = -0.45f, border_x = 0.05f, toggle_cmds_x = 0.30f, toggle_stats_x = 0.55f, btn_y = 0.8f;



bool point_in_button(float x, float y, float rx, float ry, float rw, float rh) {
    return x > rx && x < rx + rw && y > ry && y < ry + rh;
}

void draw_square(float x, float y, float w, float h, float r, float g, float b, float border = 0.01f){
    // 1) Outer colored quad
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x,       y);
    glVertex2f(x + w,   y);
    glVertex2f(x + w,   y + h);
    glVertex2f(x,       y + h);
    glEnd();

    // 2) Inner black quad inset by 'border' on each side
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(x + border,       y + border);
    glVertex2f(x + w - border,   y + border);
    glVertex2f(x + w - border,   y + h - border);
    glVertex2f(x + border,       y + h - border);
    glEnd();
}

void draw_buttons() {
    draw_square(reset_btn_x, btn_y, btn_w, btn_h, 1.0f, 1.0f, 1.0f);
    draw_square(pause_btn_x, btn_y, btn_w, btn_h, 1.0f, 1.0f, 1.0f);
    draw_square(speed_up_x, btn_y, btn_w, btn_h, 1.0f, 1.0f, 1.0f);
    draw_square(speed_down_x, btn_y, btn_w, btn_h, 1.0f, 1.0f, 1.0f);
    draw_square(border_x, btn_y, btn_w, btn_h, 1.0f, 1.0f, 1.0f);
    draw_square(toggle_cmds_x, btn_y, btn_w, btn_h, 1.0f, 1.0f, 1.0f);
    draw_square(toggle_stats_x, btn_y, btn_w, btn_h, 1.0f, 1.0f, 1.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    text_renderer.draw_text(toggle_stats_x + 0.02f, (-btn_y + btn_h / 2) - 0.1f, "Stats", screen_width, screen_height);
    text_renderer.draw_text(toggle_cmds_x + 0.02f, (-btn_y + btn_h / 2) - 0.1f, "Border", screen_width, screen_height);
    text_renderer.draw_text(border_x + 0.02f, (-btn_y + btn_h / 2) - 0.1f, "Cmds", screen_width, screen_height);
    text_renderer.draw_text(speed_down_x + 0.02f, (-btn_y + btn_h / 2) - 0.1f, "Slower", screen_width, screen_height);
    text_renderer.draw_text(speed_up_x + 0.02f, (-btn_y + btn_h / 2) - 0.1f, "Faster", screen_width, screen_height);
    text_renderer.draw_text(pause_btn_x + 0.02f, (-btn_y + btn_h / 2) - 0.1f, "Pause", screen_width, screen_height);
    text_renderer.draw_text(reset_btn_x + 0.02f, (-btn_y + btn_h / 2) - 0.1f, "Reset", screen_width, screen_height);
}

void draw_commands(){
    text_renderer.draw_text(-0.98f, -0.7f, "'load <filename>': load new object file", screen_width, screen_height);
    text_renderer.draw_text(-0.98f, -0.62f, "'clear': clear objects from screen", screen_width, screen_height);
    text_renderer.draw_text(-0.98f, -0.54, "'spawn <mass(base),mass(power),radius,vx,vy,x,y>': spawns new object", screen_width, screen_height);
    text_renderer.draw_text(-0.98f, -0.46f, "'quit': quit program", screen_width, screen_height);
}

void draw_stats(std::vector<Object> objs){
    float y = -0.7;
    float line_spacing = 0.08f;
    int index = 0;

    for (const auto& obj : objs) {
        std::ostringstream header;
        header << "Obj " << index;
        text_renderer.draw_text(-0.98f, y, header.str(), screen_width, screen_height);
        y += line_spacing;

        // Position
        std::ostringstream pos_line;
        pos_line << "  Pos: (" << to_sig_figs(obj.pos[0] / scale_factor, 4) << ", " << to_sig_figs(obj.pos[1] / scale_factor, 4) << ") km";
        text_renderer.draw_text(-0.98f, y, pos_line.str(), screen_width, screen_height);
        y += line_spacing;

        // Velocity
        std::ostringstream vel_line;
        vel_line << "  Vel: (" << to_sig_figs(obj.vel[0] / scale_factor, 4) << ", " << to_sig_figs(obj.vel[1]/ scale_factor, 4) << ") km/s";
        text_renderer.draw_text(-0.98f, y, vel_line.str(), screen_width, screen_height);
        y += 0.08;
        index++;
}
}

void init_text_renderer(){
    if (!text_renderer.init("fonts/Pixellettersfull-BnJ5.ttf")) {
        std::cout << "Failed to Load Font" << std::endl;
    }
}

void draw_command_line(std::string command_input){
    text_renderer.draw_text(-0.98f, 0.98f, ">>>", screen_width, screen_height);
    text_renderer.draw_text(-0.88f, 0.98f, command_input, screen_width, screen_height);
}