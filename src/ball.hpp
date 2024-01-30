#ifndef _BALL_HPP_
#define _BALL_HPP_

class Paddle;

class Ball
{
public:
    Vec2 position; // center of ball
    Vec2 direction;
    GLfloat size;
    GLfloat speed;
    GLboolean bounceOnce = true;
    GLboolean bounceOffWallOnce = true;

    Ball(Vec2 p, Vec2 v, GLfloat _size, GLfloat s);

    void Move(GLfloat deltaTime);

    void CheckCollisionAndBounce(Paddle &paddle);

    void BounceBall(Paddle &paddle);

    void bounceOffWall();

    void ResetBall(int dir);
};

#endif //_BALL_HPP_