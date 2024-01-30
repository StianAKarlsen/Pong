#include "defines.hpp"

#include "paddle.hpp"

Paddle::Paddle(Vec2 p, GLfloat h, GLfloat w, GLfloat s) : position(p), height(h), width(w), speed(s) {}

void Paddle::Move(int xDir, GLfloat deltaTime)
{
    position.x += xDir * speed * deltaTime;
}