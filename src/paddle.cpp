#include "defines.hpp"

#include "paddle.hpp"

Paddle::Paddle(Vec2 p, GLfloat height, GLfloat width, GLfloat s, GLint shaderProgram) : position(p), height(height), width(width), speed(s), shaderProgram(shaderProgram)
{
     GLfloat barVertices[8] = {
        width, height,
        -width, height,
        width, -height,
        -width, -height};

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(barVertices), barVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
}

void Paddle::CleanUp()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Paddle::Move(int xDir, GLfloat deltaTime)
{
    position.x += xDir * speed * deltaTime;
}

void Paddle::FollowBall(Ball &ball, GLfloat deltaTime)
{
    if (ball.position.x > position.x + width / 2)
        Move(1, deltaTime);
    else if (ball.position.x < position.x - width / 2)
        Move(-1, deltaTime);
}

void Paddle::render()
{
    glUseProgram(shaderProgram);

    glBindVertexArray(VAO);
    // glBindTexture(GL_TEXTURE_2D, textures[0]);
    // glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), GL_FALSE);
    glUniform2fv(glGetUniformLocation(shaderProgram, "modelPos"), 1, (const GLfloat *)&position);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
