#ifndef _TEXT_H_
#define _TEXT_H_

#include "defines.hpp"

class ShaderProgram;

class Text
{
private:
    struct Character
    {
        GLuint TextureID;
        glm::vec2 Size;
        glm::vec2 Bearing;
        GLuint Advance;
    };

    FT_Library ft;
    FT_Face face;
    std::map<GLchar, Character> characters;
    GLuint VAO, VBO;
    ShaderProgram *textShaderProgram;


public:
    Text();

    void LoadCharacterTextures();
    void RenderTextCenterBottom(std::string text, GLfloat x, GLfloat y, GLfloat scale);
    void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale);
    void CleanUp();
};

#endif