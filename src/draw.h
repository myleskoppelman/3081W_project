#ifndef DRAW_H
#define DRAW_H

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
#include "stb_truetype.h"

TextRenderer text_renderer;
extern float height, width;
extern float pause_btn_x, reset_btn_x, speed_up_x, btn_w, btn_h,
speed_down_x, new_sim_x, toggle_vec_x, toggle_stats_x, btn_y;


bool point_in_button(float x, float y, float rx, float ry, float rw, float rh);
void draw_square(float x, float y, float w, float h, float r, float g, float b, float border = 0.01f);
void draw_buttons();
bool valid_path(const std::string& filename);
std::vector<std:: string> tokenize(const std::string& cmd);
void draw_commands();
void draw_stats(std::vector<Object> objs);
void init_text_renderer();
void draw_command_line(std::string command_input);


#endif