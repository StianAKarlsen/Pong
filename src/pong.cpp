#include "defines.hpp"

#include "pong.hpp"

Pong::Pong(GLFWwindow *window) : window(window)
{
    // glGenFramebuffers(1, &backgroundScreenbuffer1);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLuint textFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(textFragmentShader, 1, &textFragmentShaderSource, NULL);
    glCompileShader(textFragmentShader);

    GLuint imageFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(imageFragmentShader, 1, &imageFragmentShaderSource, NULL);
    glCompileShader(imageFragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    textShaderProgram = glCreateProgram();
    glAttachShader(textShaderProgram, vertexShader);
    glAttachShader(textShaderProgram, textFragmentShader);
    glLinkProgram(textShaderProgram);

    // imageShaderProgram = glCreateProgram();
    // glAttachShader(imageShaderProgram, vertexShader);
    // glAttachShader(imageShaderProgram, imageFragmentShader);
    // glLinkProgram(imageShaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(textFragmentShader);

    // LoadTexture(startImage_png, startImage_png_len, backgroundTexture);

    // glBindFramebuffer(GL_FRAMEBUFFER, backgroundScreenbuffer1);
    // glUseProgram(imageShaderProgram);
    // glBindTexture(GL_TEXTURE_2D, backgroundTexture);
    // // glViewport(0, 0, 800, 600);
    // glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // glBindTexture(GL_TEXTURE_2D, 0);
    
    text = Text(textShaderProgram);

    playerPaddle = Paddle({0.0f, -0.9}, 0.02f, 0.3f, 0.00007f, shaderProgram);
    otherPaddle = Paddle({0.0f, 0.9}, 0.02f, 0.3f, 0.00007f, shaderProgram);

    ball = Ball(shaderProgram, {0, 0}, {0, 1}, 0.01, 0.00011f);

    lastTime = std::chrono::high_resolution_clock::now();
    currentTime = std::chrono::high_resolution_clock::now();
    deltaTime = (currentTime - lastTime).count() / 150000.0f;
}

void Pong::RenderFullScreenImage()
{

}

void Pong::GameLoop()
{
    lastTime = currentTime;
    currentTime = std::chrono::high_resolution_clock::now();
    deltaTime = (currentTime - lastTime).count() / 150000.0f;

    if (currentGameState == GameState::RUNNING)
    {
        otherPaddle.FollowBall(ball, deltaTime);
        playerPaddle.FollowBall(ball, deltaTime);

        if (ball.position.y >= 1.1)
        {
            playerScore++;
            ball.ResetBall(-1);
        }
        if (ball.position.y <= -1.1)
        {
            computerScore++;
            ball.ResetBall(1);
        }

        ball.Move(deltaTime);

        ball.CheckCollisionAndBounce(playerPaddle);
        ball.CheckCollisionAndBounce(otherPaddle);
        ball.bounceOffWall();
    }
}

void Pong::Render()
{
    if (currentGameState == GameState::START)
    {
        text.RenderText("Enter to", -0.3f, -0.03f, 0.0008f);
        text.RenderText("StaRT", -0.25f, -0.1f, 0.0008f);
        return;
    }

    playerPaddle.render();
    otherPaddle.render();
    ball.render();

    text.RenderText(std::to_string(playerScore), -0.51f, -0.03f, 0.001f);
    text.RenderText(std::to_string(computerScore), 0.51f, -0.03f, 0.001f);

    if (currentGameState == GameState::PAUSED)
        text.RenderText("Pause", -0.5f, -0.03f, 0.005f);
}

void Pong::PlayerInput()
{
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) // move playerbar
        playerPaddle.Move(-1, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) // move playerbar
        playerPaddle.Move(1, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_KP_ENTER) == GLFW_PRESS) // move playerbar
    {
        if (!enterKeyPressedOnce)
        {
            enterKeyPressedOnce = true;
            switch (currentGameState)
            {
            case GameState::RUNNING:
                currentGameState = GameState::PAUSED;
                break;
            case GameState::PAUSED:
            case GameState::START:
                currentGameState = GameState::RUNNING;
                // currentTime = std::chrono::high_resolution_clock::now();
                break;
            default:
                break;
            }
        }
    }
    else
    {
        enterKeyPressedOnce = false;
    }
}

Pong::~Pong()
{
    playerPaddle.CleanUp();
    otherPaddle.CleanUp();
    ball.CleanUp();

    // glDeleteFramebuffers(1,&backgroundScreenbuffer1);
    glDeleteProgram(shaderProgram);
    glDeleteProgram(textShaderProgram);
    glDeleteProgram(imageShaderProgram);
}
