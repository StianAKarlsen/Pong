#ifndef _DEFINES_HPP_
#define _DEFINES_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>

#include <functional>
#include <filesystem>
#include <unordered_map>

#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>

#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
#include <chrono>
#include <map>
#include <stdio.h>

#include <ft2build.h>
#include FT_FREETYPE_H



extern void LoadTexture(std::string file, GLuint &textureID);


enum struct GameState
{
    START,
    RUNNING,
    PAUSED
};





#endif //_DEFINES_HPP_