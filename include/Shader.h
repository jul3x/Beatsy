//
// Created by julex on 03.11.2020.
//

#ifndef BEATSY_SHADER_H
#define BEATSY_SHADER_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


GLuint LoadShaders(const std::string& vertex_file_path, const std::string& fragment_file_path);

#endif //BEATSY_SHADER_H
