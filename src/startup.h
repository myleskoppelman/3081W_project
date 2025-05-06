#ifndef STARTUP_H
#define STARTUP_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include "object.h"
#include "physics.h"

std::vector<Object> load_data(std::vector<Object>& objs, std::string filename);
std::string trim(const std::string &s);
bool parse_object_features(const std::string &input, Object &obj);
void parse_row(const std::vector<std::string>& tokens);
Object create_object(long double mass, float radius, std::vector<double> pos, std::vector<double> vel);
std::vector<std::string> tokenize(const std::string& cmd);
bool valid_path(const std::string& filename);
std::string to_sig_figs(double num, int sig_figs);

#endif