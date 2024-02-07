#ifndef _PONG_HPP_
#define _PONG_HPP_

#include "ball.hpp"
#include "paddle.hpp"
#include "text.hpp"
#include "ShaderProgramManager.hpp"

class Pong
{
    Ball ball;
    Paddle playerPaddle, otherPaddle;
    GLuint startTexture;
    GLuint computerScore = 0, playerScore = 0;
    
    GLuint textShaderProgram, shaderProgram, imageShaderProgram;
    ShaderProgram *defaultsp, *textsp, *imagesp;
    GameState currentGameState = GameState::START;
    
    Text text;
    GLFWwindow *window;

    std::chrono::time_point<std::chrono::high_resolution_clock> lastTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> currentTime;
    GLfloat deltaTime;

    GLboolean enterKeyPressedOnce = false;
    
    GLuint backgroundScreenbuffer1;
    GLuint VAO, VBO;
    GLuint backgroundBGTexture;
    GLuint backgroundFGTexture;

public:
    Pong(GLFWwindow *window);
    ~Pong();

    void RenderFullScreenImage(GLuint textureID);
    void GameLoop();
    void PlayerInput();
    void Render();
};

#endif //_PONG_HPP_