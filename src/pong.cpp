#include "defines.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "ShaderProgramManager.hpp"
#include "pong.hpp"

static ShaderManager &shaderManager = ShaderManager::getInstance();


void LoadTexture(std::string file, GLuint &textureID)
{
    int width, height, nrChannels;
    // unsigned char *data = stbi_load_from_memory(image, size, &width, &height, &nrChannels, 0);
    unsigned char *data = stbi_load(file.c_str(), &width, &height, &nrChannels, 0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
}


Pong::Pong(GLFWwindow *window) : window(window)
{
  auto &shaderManager = ShaderManager::getInstance();

  shaderManager.createShaderProgram(
      "DefaultShaderProgram",
      {{GL_VERTEX_SHADER, "resources/shaders/defaultVertexShader.glsl"},
       {GL_FRAGMENT_SHADER, "resources/shaders/defaultFragmentShader.glsl"}});

  shaderManager.createShaderProgram(
      "TextShaderProgram",
      {{GL_VERTEX_SHADER, "resources/shaders/defaultVertexShader.glsl"},
       {GL_FRAGMENT_SHADER, "resources/shaders/textFragmentShader.glsl"}});

  shaderManager.createShaderProgram(
      "ImageShaderProgram",
      {{GL_VERTEX_SHADER, "resources/shaders/defaultVertexShader.glsl"},
       {GL_FRAGMENT_SHADER, "resources/shaders/imageFragmentShader.glsl"}});

  defaultShaderProgram = shaderManager.getShaderProgram("DefaultShaderProgram");
  imagesp = shaderManager.getShaderProgram("ImageShaderProgram");

  glGenTextures(1, &backgroundBGTexture);
  glGenTextures(1, &backgroundFGTexture);

  LoadTexture("resources/StartScreenBG.png", backgroundBGTexture);
  LoadTexture("resources/StartScreenFG.png", backgroundFGTexture);

  GLfloat fullscreenVertices[4][4] = {
      // x,    y,    texture x, texture y
      {-1.0f, -1.0f, 0.0f, 1.0f}, // Bottom-left
      {-1.0f, 1.0f, 0.0f, 0.0f},  // Top-left
      {1.0f, -1.0f, 1.0f, 1.0f},  // Bottom-right
      {1.0f, 1.0f, 1.0f, 0.0f}    // Top-right
  };

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(fullscreenVertices), fullscreenVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (const void *)(2 * sizeof(GLfloat)));

  // glGenFramebuffers(1, &fbo);
  // glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  // // Create a texture to hold the framebuffer content
  // // glGenTextures(1, &backgroundBGTexture);
  // // glBindTexture(GL_TEXTURE_2D, fbo);
  // // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo, 0);
  // // glBindTexture(backgroundBGTexture); //???
  // // Attach texture to the framebuffer
  // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, backgroundBGTexture, 0);
  // glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // glUseProgram(0);



  playerPaddle = Paddle({0.04f, -0.9}, 0.02f, 0.3f, 0.00007f);
  otherPaddle = Paddle({0.00f, 0.9}, 0.02f, 0.3f, 0.00007f);

  ball = Ball({0, 0}, {0, 1}, 0.01, 0.00011f);

  lastTime = std::chrono::high_resolution_clock::now();
  currentTime = std::chrono::high_resolution_clock::now();
  deltaTime = (currentTime - lastTime).count() / 150000.0f;
}

void Pong::RenderFullScreenImage(GLuint textureID)
{
  static const GLfloat pos[] = {0, 0};
  imagesp->use();
  imagesp->setTexture("screenTexture",textureID, 0);
  imagesp->setUniform("modelPos", pos);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  // glBindFramebuffer(GL_FRAMEBUFFER, backgroundScreenbuffer1);
  // glClearColor(0.1f, 0.2f, 0.2f, 1.0f);
  // glClear(GL_COLOR_BUFFER_BIT);
  // glBindFramebuffer(GL_FRAMEBUFFER, 0);
  // glDrawArrays(GL_TRIANGLES, 0, 0);
  // glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Pong::GameLoop()
{
  lastTime = currentTime;
  currentTime = std::chrono::high_resolution_clock::now();
  deltaTime = (currentTime - lastTime).count() / 150000.0f;

  if (currentGameState == GameState::START)
    playerPaddle.FollowBall(ball, deltaTime);
  if (currentGameState != GameState::PAUSED)
  {

    otherPaddle.FollowBall(ball, deltaTime);

    if (ball.position.y >= 1.1)
    {
      if (currentGameState != GameState::START)
        playerScore++;
      ball.ResetBall(-1);
    }
    if (ball.position.y <= -1.1)
    {
      if (currentGameState != GameState::START)
        computerScore++;
      ball.ResetBall(1);
    }

    ball.CheckCollisionAndBounce(playerPaddle);
    ball.CheckCollisionAndBounce(otherPaddle);
    ball.bounceOffWall();
    ball.Move(deltaTime);
  }
}

void Pong::Render()
{
  // glBindFramebuffer(GL_FRAMEBUFFER, fbo)
  RenderFullScreenImage(backgroundBGTexture);
  // glBindFramebuffer(GL_FRAMEBUFFER, 0)
  defaultShaderProgram->use();
  defaultShaderProgram->setTexture("screenTexture", backgroundBGTexture, 0);

  playerPaddle.render();
  otherPaddle.render();
  ball.render();

  if (currentGameState == GameState::START)
  {
    text.RenderText("Enter to", -0.0f, -0.78f, 0.0008f);
    // text.RenderText("StaRT", -0.0f, -0.8f, 0.0008f);
  }
  text.RenderText(std::to_string(playerScore), -0.51f, -0.03f, 0.001f);
  text.RenderText(std::to_string(computerScore), 0.51f, -0.03f, 0.001f);

  // if (currentGameState == GameState::START)
  // RenderFullScreenImage(backgroundFGTexture);
  if (currentGameState == GameState::PAUSED)
    text.RenderText("Pause", -0.5f, -0.03f, 0.005f);
}

void Pong::PlayerInput()
{
  if (currentGameState == GameState::RUNNING)
  {
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) // move playerbar
      playerPaddle.Move(-1, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) // move playerbar
      playerPaddle.Move(1, deltaTime);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) // move playerbar
    {
        // ball.ResetBall(-1);
      glfwGetCursorPos(window, (double*)&ball.position.x, (double*)&ball.position.y);
    }
  }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // move playerbar
      glfwSetWindowShouldClose(window, GL_TRUE);
  // if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
  //   shaderManager;

  if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS ||
      glfwGetKey(window, GLFW_KEY_KP_ENTER) == GLFW_PRESS)
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
        currentGameState = GameState::RUNNING;
        break;
      case GameState::START:
        currentGameState = GameState::RUNNING;
        otherPaddle.position.x = 0;
        playerPaddle.position.x = 0;
        ball.ResetBall(-1);

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
  text.CleanUp();

  glDeleteTextures(1, &backgroundBGTexture);

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

  // glDeleteProgram(shaderProgram);
  // glDeleteProgram(textShaderProgram);
  // glDeleteProgram(imageShaderProgram);
}
