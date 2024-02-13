#ifndef _PADDLE_HPP_
#define _PADDLE_HPP_

#include "ShaderProgramManager.hpp"
#include "ball.hpp"

class Paddle
{
    GLuint VAO, VBO;
    ShaderProgram *defaultsp;

public:
    glm::vec2 position; // center of paddel
    GLfloat height, width;
    GLfloat speed;
    Paddle() {};
    Paddle(glm::vec2 p, GLfloat h, GLfloat w, GLfloat s);
    void CleanUp();
    void Move(int xDir, GLfloat deltaTime);
    void FollowBall(Ball &ballm, GLfloat deltaTime);
    void render();
};

#endif //_PADDLE_HPP_