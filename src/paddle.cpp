#include "defines.hpp"

#include "paddle.hpp"

Paddle::Paddle(Vec2 p, GLfloat h, GLfloat w, GLfloat s, GLint shaderProgram) : position(p), height(h), width(w), speed(s), shaderProgram(shaderProgram)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(barVertices), barVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);
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
    glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), GL_FALSE);
    glUniform2fv(0, 1, (const GLfloat *)&position);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
