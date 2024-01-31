#ifndef _PADDLE_HPP_
#define _PADDLE_HPP_

#include "ball.hpp"

class Paddle
{
    GLuint VAO, VBO;

public:
    Vec2 position; // center of paddel
    GLfloat height, width;
    GLfloat speed;
    GLint shaderProgram;

    Paddle() {};
    Paddle(Vec2 p, GLfloat h, GLfloat w, GLfloat s, GLint shaderProgram);

    void Move(int xDir, GLfloat deltaTime);
    void FollowBall(Ball &ballm, GLfloat deltaTime);
    void render();
};

#endif //_PADDLE_HPP_