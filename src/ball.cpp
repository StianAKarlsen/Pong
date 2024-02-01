#include "defines.hpp"

#include "paddle.hpp"
#include "ball.hpp"

Ball::Ball(GLint shaderProgram, Vec2 p, Vec2 v, GLfloat _size, GLfloat s) : shaderProgram(shaderProgram), position(p), direction(v), size(_size), speed(s)
{
    GLfloat ballVertices[8] = {
        _size, _size,
        -_size, _size,
        _size, -_size,
        -_size, -_size};

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ballVertices), ballVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
}

void Ball::CleanUp()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

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

void Ball::render()
{
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glUniform2fv(0, 1, (const GLfloat *)&position);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
