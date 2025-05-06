#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <filesystem>
#include "object.h"
#include "physics.h"
#include "globals.h"



Object create_object(long double mass, float radius, std::vector<double> pos, std::vector<double> vel){
    return Object(radius, mass, pos , vel);
}


void parse_row(const std::vector<std::string>& tokens, std::vector<Object>& objs) {
    if (tokens.size() != 7) {
        std::cerr << "Error: Expected 7 values, but got " << tokens.size() << std::endl;
        return;
    }

    try {
        double base = std::stod(tokens[0]);  
        double exponent = std::stod(tokens[1]);  
        long double mass = base * std::pow(10, exponent);

        float radius = std::stof(tokens[2] );  
        std::vector<double> velocity = { std::stod(tokens[3]) * scale_factor, std::stod(tokens[4]) * scale_factor};
        std::vector<double> position = { std::stod(tokens[5]) * scale_factor, std::stod(tokens[6]) * scale_factor};

        Object obj = create_object(mass, radius, velocity, position);
        objs.push_back(obj);
        std::cout << "Object Loaded" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: Invalid number format -> " << e.what() << std::endl;
    }
}


std::string trim(const std::string &s) {
    auto start = s.begin();
    while (start != s.end() && isspace(*start)) {
        ++start;
    }
    
    auto end = s.end();
    do {
        --end;
    } while (distance(start, end) > 0 && isspace(*end));
    
    return std::string(start, end + 1);
}


bool parse_object_features(const std::string &input, Object &obj) {
    std::istringstream ss(input);
    std::string token;
    std::vector<std::string> tokens;
    
    while (std::getline(ss, token, ',')) {
        token = trim(token);
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }

    if (tokens.size() != 7) {
        std::cerr << "Error: You must enter exactly six values separated by commas." << std::endl;
        return false;
    }
    double base = std::stod(tokens[0]);  
    double exponent = std::stod(tokens[1]);  

    long double mass = base * pow(10, exponent);
    std::vector<double> vel = { std::stod(tokens[3]) * scale_factor, std::stod(tokens[4]) * scale_factor };
    std::vector<double> pos = { std::stod(tokens[5]) * scale_factor, std::stod(tokens[6]) * scale_factor };


    try {
        obj.m = mass;         
        obj.r = std::stof(tokens[2]);       
        obj.vel = vel;
        obj.pos = pos;
    } catch (const std::invalid_argument&) {
        std::cerr << "Error: Invalid number format." << std::endl;
        return false;
    }

    return true;
}


std::vector<Object> load_data(std::vector<Object>& objs, std::string filename) {

    std::ifstream file;
    std::transform(filename.begin(), filename.end(), filename.begin(), ::tolower);
    file.open(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
    }

    std::string line;
    std::vector<std::vector<std::string>> data;

    while (std::getline(file, line)) {
        std::vector<std::string> tokens;
        std::stringstream ss(line);
        std::string cell;
        std::cout << "Reading file..." << std::endl;

        while (std::getline(ss, cell, ',')) {
            tokens.push_back(cell);
        }
        (parse_row(tokens, objs));
        
    }
    file.close();
    std::cout << "Objects Loaded" << std::endl;
    return objs;
}


std::vector<std::string> tokenize(const std::string& cmd) {
    std::istringstream iss(cmd);
    std::vector<std::string> tokens;
    std::string word;
    while (iss >> word) {
        tokens.push_back(word);
    }
    return tokens;
}


bool valid_path(const std::string& filename) {
    return std::filesystem::exists(filename) && std::filesystem::is_regular_file(filename);
}


std::string to_sig_figs(double num, int sig_figs = 3) {
    if (num == 0.0) return "0";
    int exp = static_cast<int>(std::floor(std::log10(std::fabs(num))));
    std::ostringstream out;
    out << std::fixed << std::setprecision(sig_figs - exp - 1) << num;
    return out.str();
}
