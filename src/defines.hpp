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

#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
#include <chrono>
#include <map>
#include <stdio.h>

#include <ft2build.h>
#include FT_FREETYPE_H

extern unsigned char bounceSound_wav[];
extern unsigned int bounceSound_wav_len;

// extern unsigned char backgroundImage_png[];
// extern unsigned int backgroundImage_png_len;

// extern unsigned char pauseImage_png[];
// extern unsigned int pauseImage_png_len;

// extern unsigned char startImage_png[];
// extern unsigned int startImage_png_len;

// extern const char *defaultVertexShaderSource;
// extern const char *defaultFragmentShaderSource;
// extern const char *textFragmentShaderSource;
// extern const char *imageFragmentShaderSource;

extern void LoadTexture(std::string file, GLuint &textureID);
// class ShaderManager;
// extern ShaderManager &shaderManager;;

// class ShaderProgramManager;
// extern ShaderProgramManager g_shaderProgramManager;

enum struct GameState
{
    START,
    RUNNING,
    PAUSED
};

struct iVec2
{
    int x, y;
};
struct Character
{
    GLuint TextureID;
    iVec2 Size;
    iVec2 Bearing;
    GLuint Advance;
};

struct Vec2
{
    GLfloat x, y;
};

#endif //_DEFINES_HPP_