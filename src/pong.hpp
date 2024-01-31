#ifndef _PONG_HPP_
#define _PONG_HPP_

#include "ball.hpp"
#include "paddle.hpp"
#include "text.hpp"

class Pong
{
    Ball ball;
    Paddle player, other;
    GLuint startTexture;
    GLuint computerScore = 0, playerScore = 0;
    
    GameState currentGameState = GameState::START;
    
    Text text;
    GLFWwindow *window;

    std::chrono::time_point<std::chrono::high_resolution_clock> lastTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> currentTime;
    GLfloat deltaTime;

    GLboolean enterKeyPressedOnce = false;

public:
    Pong(GLFWwindow *window);
    
    void GameLoop();
    void PlayerInput();
    void Render();
};

#endif //_PONG_HPP_