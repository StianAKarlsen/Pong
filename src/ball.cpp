#include "defines.hpp"

#include "paddle.hpp"
#include "ball.hpp"


Ball::Ball(Vec2 p, Vec2 v, GLfloat _size, GLfloat s) : position(p), direction(v), size(_size), speed(s) {}

void Ball::Move(GLfloat deltaTime)
{
    position.x += speed * direction.x * deltaTime;
    position.y += speed * direction.y * deltaTime;
}

void Ball::CheckCollisionAndBounce(Paddle &paddle)
{
    if (position.x + size >= paddle.position.x - paddle.width &&
        position.x - size <= paddle.position.x + paddle.width &&
        position.y + size >= paddle.position.y - paddle.height &&
        position.y - size <= paddle.position.y + paddle.height)
    {
        if (bounceOnce)
        {
            PlaySound((LPCSTR)bounceSound_wav, NULL, SND_MEMORY | SND_ASYNC);
            // PlaySound(TEXT("bounceSound0.wav"), NULL, SND_FILENAME | SND_ASYNC);
            // MessageBeep(MB_ICONWARNING);
            BounceBall(paddle);
            bounceOnce = false;
        }
    }
    else
    {
        bounceOnce = true;
    }
}

void Ball::BounceBall(Paddle &paddle)
{
    direction.y = -direction.y;

    float hitPoint = (position.x - paddle.position.x) / (paddle.width / 2.0f);
    float angle = (hitPoint * 45.0f) * (M_PI / 180.0f);
    direction.x = sin(angle);
}

void Ball::bounceOffWall()
{
    if (position.x + size >= 1 || position.x - size <= -1)
    {
        if (bounceOffWallOnce)
        {
            PlaySound((LPCSTR)bounceSound_wav, NULL, SND_MEMORY | SND_ASYNC);
            // PlaySound(TEXT("bounceSound0.wav"), NULL, SND_FILENAME | SND_ASYNC);
            direction.x *= -1;
            bounceOffWallOnce = false;
        }
    }
    else
    {
        bounceOffWallOnce = true;
    }
}

void Ball::ResetBall(int dir)
{
    position.x = 0;
    position.y = 0;
    direction.x = 0;
    direction.y = dir;
}