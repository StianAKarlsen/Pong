#ifndef _TEXT_H_
#define _TEXT_H_

#include "defines.hpp"

class Text
{
private:
    FT_Library ft;
    FT_Face face;
    std::map<GLchar, Character> characters;
    GLuint  VAO, VBO;
    GLuint textShaderProgram;

public:
    Text() {};
    Text(GLuint shaderProgram);

    void LoadCharacterTextures();
    void RenderTextCenterBottom(std::string text, GLfloat x, GLfloat y, GLfloat scale);
    void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale);
    void CleanUp();
};

#endif