#ifndef _PADDLE_HPP_
#define _PADDLE_HPP_

class Paddle
{
public:
    Vec2 position; // center of paddel
    GLfloat height;
    GLfloat width;
    GLfloat speed;

    Paddle(Vec2 p, GLfloat h, GLfloat w, GLfloat s);

    void Move(int xDir, GLfloat deltaTime);
};

#endif //_PADDLE_HPP_