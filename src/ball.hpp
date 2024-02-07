#ifndef _BALL_HPP_
#define _BALL_HPP_

#include "ShaderProgramManager.hpp"
class Paddle;

class Ball
{
    GLuint VAO, VBO;
    GLint shaderProgram;
    ShaderProgram *defaultsp;

public:
    Vec2 position; // center of ball
    Vec2 direction;
    GLfloat size;
    GLfloat speed;
    GLboolean bounceOnce = true;
    GLboolean bounceOffWallOnce = true;


private:


public:
    Ball(){};
    Ball(GLint shaderProgram, Vec2 p, Vec2 v, GLfloat _size, GLfloat s);
    void CleanUp();
    void Move(GLfloat deltaTime);

    void CheckCollisionAndBounce(Paddle &paddle);
    void BounceBall(Paddle &paddle);
    void bounceOffWall();
    void ResetBall(int dir);

    void render();
};

#endif //_BALL_HPP_