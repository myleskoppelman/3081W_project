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
#include "draw.h"
#include "stb_truetype.h"
#include "globals.h"


float screen_height = 500, screen_width = 800;
bool cmds = false, stats = false, border = false, reset = false, pause = true, button_clicked = false;
float sim_speed = 1.00f, fps = 1/60.0f, updates_per_frame = 20.0f, scale_factor = 1.0f / 1000000.0f;
std::string command_input;
std::vector<Object> objs, objs_backup; 



void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        int width, height;
        glfwGetWindowSize(window, &width, &height);

        float norm_x = (xpos / width) * 2.0f - 1.0f;
        float norm_y = 1.0f - (ypos / height) * 2.0f;

        if (point_in_button(norm_x, norm_y, pause_btn_x, btn_y, btn_w, btn_h)) {
            pause = !pause; 
        } else if (point_in_button(norm_x, norm_y, reset_btn_x, btn_y, btn_w, btn_h)) {
            reset = !reset;
        } else if (point_in_button(norm_x, norm_y, speed_up_x, btn_y, btn_w, btn_h)) {
            sim_speed = sim_speed * 2.0f;
        } else if (point_in_button(norm_x, norm_y, speed_down_x, btn_y, btn_w, btn_h)) {
            sim_speed = sim_speed / 2.0f;
        } else if (point_in_button(norm_x, norm_y, border_x, btn_y, btn_w, btn_h)) {
            cmds = !cmds;
        } else if (point_in_button(norm_x, norm_y, toggle_stats_x, btn_y, btn_w, btn_h)) {
            stats = !stats;
        } else if (point_in_button(norm_x, norm_y, toggle_cmds_x, btn_y, btn_w, btn_h)) {
            border = !border;
        }
        
    }
}

void char_callback(GLFWwindow* window, unsigned int codepoint) {
    command_input += static_cast<char>(codepoint);
}

void process_command(const std::string& cmd, GLFWwindow* window) {
    std::vector<std::string> tokens = tokenize(cmd);
    if (tokens.empty()) return;

    std::string command = tokens[0];
    std::transform(command.begin(), command.end(), command.begin(), ::tolower);

    if (command == "load") {
        pause = true;
        if (!valid_path(tokens[1])){
        }
        else {
            objs = load_data(objs, tokens[1]);
            objs_backup = objs;
        }
    } 
    if (command == "clear"){
        pause = true;
        objs.clear();
    }
    if (command == "spawn"){
        Object obj;
        if (!parse_object_features(tokens[1], obj)){
        } else{
            objs.push_back(obj);
        }
    }
    if (command == "quit"){
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (command == "list"){

    }
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (key == GLFW_KEY_BACKSPACE && !command_input.empty()) {
            command_input.pop_back();
        } else if (key == GLFW_KEY_ENTER) {
            process_command(command_input, window);
            command_input.clear();
            
        }
    }
}

GLFWwindow* StartGLU() {
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW, panic" << std::endl;
        return nullptr;
    }

    GLFWmonitor* primary = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primary);
    int screen_width = mode->width;
    int screen_height = mode->height;

    // Create windowed mode window with decorations
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    GLFWwindow* window = glfwCreateWindow(screen_width, screen_height, "2D_TEST", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);

    // Show window and poll events to allow framebuffer to be initialized properly
    glfwShowWindow(window);
    glfwPollEvents();

    // Now get correct framebuffer size and set viewport
    int fb_width, fb_height;
    glfwGetFramebufferSize(window, &fb_width, &fb_height);
    glViewport(0, 0, fb_width, fb_height);

    // GLEW initialization
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW." << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return nullptr;
    }

    // Set up 2D projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    return window;
}

int main() {
    GLFWwindow* window = StartGLU();
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCharCallback(window, char_callback);
    init_text_renderer();
    

    pause = true;

    // std::vector<Object> objs = {
        // Object(0.01f, 7.0 * pow(10, 22), {0.5f, 0.0f}, {0.0f, 0.0f}),
        // Object(0.01f, 7.0 * pow(10, 22), {-0.5f, 0.0f}, {0.0f, 0.0f}),
        // Object(0.01f, 7.0 * pow(10, 22), {0.5f, 0.03f}, {0.0f, 0.005f}),
        // Object(0.01f, 7.0 * pow(10, 22), {-0.5f, -0.3f}, {0.0f, -0.005f}),
        // Object(0.01f, 7.34 * pow(10, 22), {0.0f, 0.7f}, {-0.005f, 0.0f}), //moon
        // Object(0.05f, 5.9 * pow(10, 24), {0.0f, 0.0f}, {-0.0f, 0.0f}) // earth
        // Object(1737.0f * scale_factor, 7.34 * pow(10, 22), {0.5f, 0.0f}, {0.0f, 0.0f}), // Moon
        // Object(1737.0f * scale_factor, 7.34 * pow(10, 22), {384000.0f * scale_factor, 0.0f}, {0.0f, 0.05f}), // Moon
        // Object(1737.0f * scale_factor, 7.34 * pow(10, 22), {-384000.0f * scale_factor, 0.0f}, {0.0f, -0.05f}), // Moon
        // Object(1737.0f * scale_factor, 7.34 * pow(10, 22), {0.0f, 384000.0f * scale_factor}, {-0.05f, 0.0f}), // Moon
        // Object(1737.0f * scale_factor, 7.34 * pow(10, 22), {0.0f, -384000.0f * scale_factor}, {0.05f, -0.0f}), // Moon
        // Object(6378.0f * scale_factor, 5.9 * pow(10, 24), {0.0f, 0.0f}, {0.0f, 0.0f}), // Earth
        // Object(1737.0f * scale_factor, 7.34 * pow(10, 20), {0.0f, 200000.0f * scale_factor}, {-0.07f, 0.0f}), // Moon
        // Object(0.01f, 7.3 * pow(10, 22), {0.0f, -384000.0f * scale_factor}, {1023.0f * scale_factor, 0.0f}), // Moon
        // Object(0.03f, 5.9 * pow(10, 24), {0.0f, 0.0f}, {0.0f, 0.0f}), // Earth
        // Object(0.01f, 7.34 * pow(10, 26), {-1000000.0f * scale_factor, 500000.0f * scale_factor}, {100000 * scale_factor, -0.0f}),
        // Object(0.01f, 7.34 * pow(10, 26), {-1000000.0f * scale_factor, -500000.0f * scale_factor}, {100000 * scale_factor, -0.0f})
    // };

    

    // while (true) {
    //     glfwPollEvents(); // Process input
    //     if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    //         break; // Exit loop and start the simulation
    //     }
    // }

    double last_time = glfwGetTime();
    double last_time_2 = glfwGetTime();
    double start_time = glfwGetTime();
    double current_time = glfwGetTime();
  
    while (!glfwWindowShouldClose(window)) {

        if (!pause){
            double current_time = glfwGetTime(); 
            double physics_timestep = fps / (updates_per_frame * sim_speed);   // 1 / 60 * 1 * 20 = 1 / 1200 
                                                                                      // 1 / 60 * 10 * 20 = 12000
            if (current_time - last_time >= physics_timestep) {
                last_time = current_time;
    
                for(auto& obj: objs){
                    for (auto& obj2: objs){
                        if (&obj2 == &obj) {continue;};
                        double dx = (obj2.pos[0] - obj.pos[0]);
                        double dy = (obj2.pos[1] - obj.pos[1]);
                        double distance = calculate_distance(dx, dy);  // gl units
    
                        if (distance > obj.r + obj2.r){
    
                            std::vector<double> direction = calculate_direction(dx, dy, distance); // gl units
    
                            distance *= (1 / (scale_factor)); // kms
                            distance *= 1000; // meters
    
                            long double total_acc = calculate_g_force(obj.m, obj2.m, distance) / (obj.m);  // m/s^2
                            total_acc /= 1000;
                            std::vector<long double> acc = {(total_acc * direction[0]),  (total_acc * direction[1])}; // gl m/s^2
    
                            
                            if (!pause && total_acc != INFINITY){
                                obj.acc(acc[0] * delta_t, acc[1] * delta_t); // gl m/s^2
                            }
                        }
                    }
                    if (!pause){
                        obj.move();
                    }
                    obj.check_collision(objs);
                    if (border){
                        obj.check_boundry();
                    }
                }
            }
        }

        glClear(GL_COLOR_BUFFER_BIT);
        current_time = glfwGetTime();

        if (current_time - last_time_2 >= fps) {
            last_time_2 = current_time;
            for (auto& obj: objs){
                glColor3f(1.0f, 1.0f, 1.0f);
                obj.draw();
            }
        draw_buttons();
        draw_square(-1.0f, -1.0f, 2.0f, 0.1f, 1.0f, 1.0f, 1.0f); 
        glColor3f(1.0f, 1.0f, 1.0f);
        draw_command_line(command_input);
        if (cmds){
            draw_commands();
            
        }
        if (stats){
            draw_stats(objs);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
        }

        if (reset){
            objs = objs_backup;
            pause = true;
            glClear(GL_COLOR_BUFFER_BIT);
            for (auto& obj: objs){
                obj.draw();
            }
            draw_buttons();
        }
        reset = false;

    
        
    }
    // std::cout << "i: " << i << std::endl;
    // std::cout << "i: "<<  fps / (sim_speed * updates_per_frame) << std::endl;
    // std::cout << "dt: " << delta_t << std::endl;
    // std::cout << "timestep: " << fps / updates_per_frame<< std::endl;
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}


// 20 * 30 * 1x = 600
// 20/2x * 30 * 2x = 600
// 20/0.5x * 30 * 0.5x = 600
// draw: updates at fps
// physics: updates at fps/speed
