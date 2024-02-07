#include "defines.hpp"

#include "ShaderProgramManager.hpp"
#include "pong.hpp"

ShaderManager &shaderManager = ShaderManager::getInstance();


Pong::Pong(GLFWwindow *window) : window(window)
{
  // std::string path;
  // GLenum type;
  // time_t lastModified = 0;

  // ShaderFile defaultVertexShader1{"resources/shaders/defaultVertexShader.glsl", GL_VERTEX_SHADER};

  // std::vector<ShaderFile> defaultShaderProgramShaderList{
  //     defaultVertexShader1,
  //     {"resources/shaders/defaultFragmentShader.glsl", GL_FRAGMENT_SHADER}};

  // std::vector<ShaderFile> textShaderProgramShaderList{
  //     defaultVertexShader1,
  //     {"resources/shaders/textFragmentShader.glsl", GL_FRAGMENT_SHADER}};

  // std::vector<ShaderFile> imageShaderProgramShaderList{
  //     defaultVertexShader1,
  //     {"resources/shaders/imageFragmentShader.glsl", GL_FRAGMENT_SHADER}};

  // textShaderProgram = g_shaderProgramManager.registerShaderProgram(textShaderProgramShaderList);
  // shaderProgram = g_shaderProgramManager.registerShaderProgram(defaultShaderProgramShaderList);
  // imageShaderProgram = g_shaderProgramManager.registerShaderProgram(imageShaderProgramShaderList);
    
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
       
  // defaultsp = shaderManager.getShaderProgram("DefaultShaderProgram");
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

  // glUseProgram(0);

  text = Text(textShaderProgram);

  playerPaddle = Paddle({0.04f, -0.9}, 0.02f, 0.3f, 0.00007f, shaderProgram);
  otherPaddle = Paddle({0.00f, 0.9}, 0.02f, 0.3f, 0.00007f, shaderProgram);

  ball = Ball(shaderProgram, {0, 0}, {0, 1}, 0.01, 0.00011f);

  lastTime = std::chrono::high_resolution_clock::now();
  currentTime = std::chrono::high_resolution_clock::now();
  deltaTime = (currentTime - lastTime).count() / 150000.0f;
}

void Pong::RenderFullScreenImage(GLuint textureID)
{
  // glUseProgram(imageShaderProgram);
  GLuint programID = imagesp->use();

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureID);
  glUniform1i(glGetUniformLocation(programID, "screenTexture"), 0);
  static const GLfloat pos[] = {0, 0};
  glUniform2fv(glGetUniformLocation(programID, "modelPos"), 1, pos);
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

  RenderFullScreenImage(backgroundBGTexture);
  if (currentGameState == GameState::START)
  {
    text.RenderText("Enter to", -0.0f, -0.78f, 0.0008f);
    // text.RenderText("StaRT", -0.0f, -0.8f, 0.0008f);
  }

  playerPaddle.render();
  otherPaddle.render();
  ball.render();

  text.RenderText(std::to_string(playerScore), -0.51f, -0.03f, 0.001f);
  text.RenderText(std::to_string(computerScore), 0.51f, -0.03f, 0.001f);

  // if (currentGameState == GameState::START)
  RenderFullScreenImage(backgroundFGTexture);
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
  }

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
