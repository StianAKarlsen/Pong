#ifndef _TEXT_H_
#define _TEXT_H_

#include "defines.hpp"

class Text
{
private:
    FT_Library ft;
    FT_Face face;
    std::map<GLchar, Character> Characters;
    GLuint  VAO, VBO;
    GLuint textShaderProgram;

public:
    Text(GLuint shaderProgram);
    
    void LoadCharacterTextures();
    void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale);
  
};

#endif