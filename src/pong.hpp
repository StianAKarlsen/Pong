#ifndef _PONG_HPP_
#define _PONG_HPP_

#include "ball.hpp"
#include "paddle.hpp"
#include "text.hpp"
#include "ShaderProgramManager.hpp"
#include "Window.hpp"

class Window;
// class Ball;
// class Text;
// class Paddle;

class Pong
{
    Ball ball;
    Paddle playerPaddle, otherPaddle;
    GLuint startTexture;
    GLuint computerScore = 0, playerScore = 0;

    // GLuint textShaderProgram, shaderProgram, imageShaderProgram;
    ShaderProgram *defaultShaderProgram, *textsp, *imagesp;
    GameState currentGameState = GameState::START;

    Window m_window;
    GLFWwindow *defaultGLFWwindow;
    
    Text text;

    std::chrono::time_point<std::chrono::high_resolution_clock> lastTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> currentTime;
    GLfloat deltaTime;

    GLboolean enterKeyPressedOnce = false;

    GLuint backgroundScreenbuffer1;
    GLuint VAO, VBO;
    GLuint backgroundBGTexture;
    GLuint backgroundFGTexture;
    GLuint fbo;

public:
    Pong();
    ~Pong();

    void MainLoop();
    void RenderFullScreenImage(GLuint textureID);
    void GameLoop();
    void PlayerInput();
    void Render();
};

#endif //_PONG_HPP_