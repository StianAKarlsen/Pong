#include "defines.hpp"

#include "paddle.hpp"
#include "ball.hpp"
#include "text.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

GLuint playerScore = 0, computerScore = 0;
GLuint VAO[TOTAL_VAO], VBO[TOTAL_VBO], textures[TOTAL_TEXTURES];
GLuint shaderProgram, textShaderProgram;
GLint modelPos, textModelPos;
bool enterKeyPressedOnce = false;

GameState currentGameState = GameState::START;

Paddle playerPaddle({0.0f, -0.9}, 0.02f, 0.3f, 0.00007f);
Paddle otherPaddle({0.0f, 0.9}, 0.02f, 0.3f, 0.00007f);

Ball ball({0, 0}, {0, 1}, 0.01, 0.00011f);

GLfloat fullScreenVertices[] = {
    -1.0f, 1.0f, 0.0f, 0.0f,
    -1.0f, -1.0f, 0.0f, 1.0f,
    1.0f, -1.0f, 1.0f, 1.0f,

    -1.0f, 1.0f, 0.0f, 0.0f,
    1.0f, -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 0.0f};

GLfloat barVertices[] = {
    playerPaddle.width, playerPaddle.height, 0.0f,
    -playerPaddle.width, playerPaddle.height, 0.0f,
    playerPaddle.width, -playerPaddle.height, 0.0f,
    -playerPaddle.width, -playerPaddle.height, 0.0f};

GLfloat ballVertices[] = {
    ball.size, ball.size, 0.0f,
    -ball.size, ball.size, 0.0f,
    ball.size, -ball.size, 0.0f,
    -ball.size, -ball.size, 0.0f};

void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void renderGame()
{
    glUseProgram(shaderProgram);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glUniform1i(glGetUniformLocation(shaderProgram, "textureSampler"), 0);

    glBindVertexArray(VAO[2]);
    static const GLfloat pos[] = {0, 0};
    glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), GL_TRUE);

    glUniform2fv(modelPos, 1, pos);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindVertexArray(VAO[0]);
    // glBindTexture(GL_TEXTURE_2D, textures[0]);
    glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), GL_FALSE);
    glUniform2fv(modelPos, 1, (const GLfloat *)&playerPaddle.position);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glUniform2fv(modelPos, 1, (const GLfloat *)&otherPaddle.position);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindVertexArray(VAO[1]);
    glUniform2fv(modelPos, 1, (const GLfloat *)&ball.position);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindVertexArray(0);
};

void renderStartScreen()
{
    glUseProgram(shaderProgram);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[2]);
    glUniform1i(glGetUniformLocation(shaderProgram, "textureSampler"), 0);

    glBindVertexArray(VAO[2]);
    static const GLfloat pos[] = {0, 0};
    glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), GL_TRUE);

    glUniform2fv(modelPos, 1, pos);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
};

void renderPauseScreen()
{
    renderGame();
    glUseProgram(shaderProgram);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glUniform1i(glGetUniformLocation(shaderProgram, "textureSampler"), 0);

    glBindVertexArray(VAO[2]);
    static const GLfloat pos[] = {0, 0};
    glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), GL_TRUE);

    glUniform2fv(modelPos, 1, pos);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
};

void LoadTexture(unsigned char *image, unsigned int size, GLuint textureID)
{
    int width, height, nrChannels;
    unsigned char *data = stbi_load_from_memory(image, size, &width, &height, &nrChannels, 0);
    // unsigned char *data = stbi_load(file.c_str(), &width, &height, &nrChannels, 0);
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

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow(640, 480, "PONG", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glewInit();
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSwapInterval(1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // glEnable(GL_DEPTH_TEST);
    // glPolygonMode(GL_BACK, GL_FILL);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLuint textFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(textFragmentShader, 1, &textFragmentShaderSource, NULL);
    glCompileShader(textFragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLuint textShaderProgram = glCreateProgram();
    glAttachShader(textShaderProgram, vertexShader);
    glAttachShader(textShaderProgram, textFragmentShader);
    glLinkProgram(textShaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(textFragmentShader);

    modelPos = glGetUniformLocation(shaderProgram, "modelPos");
    // textModelPos = glGetUniformLocation(textShaderProgram, "modelPos");
    Text text(textShaderProgram);

    text.LoadCharacterTextures();

    glGenVertexArrays(TOTAL_VAO, VAO);
    glGenBuffers(TOTAL_VBO, VBO);

    glGenTextures(TOTAL_TEXTURES, textures);

    LoadTexture(backgroundImage_png, backgroundImage_png_len, textures[0]);
    LoadTexture(pauseImage_png, pauseImage_png_len, textures[1]);
    LoadTexture(startImage_png, startImage_png_len, textures[2]);

    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(barVertices), barVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ballVertices), ballVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    glBindVertexArray(VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fullScreenVertices), fullScreenVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *)(2 * sizeof(GLfloat)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    auto lastTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    GLfloat deltaTime = (currentTime - lastTime).count() / 150000.0f;

    glUseProgram(shaderProgram);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (currentGameState == GameState::RUNNING)
        {
            lastTime = currentTime;
            currentTime = std::chrono::high_resolution_clock::now();
            deltaTime = (currentTime - lastTime).count() / 150000.0f;

            renderGame();

            ball.bounceOffWall();

            ball.CheckCollisionAndBounce(playerPaddle);
            ball.CheckCollisionAndBounce(otherPaddle);

            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) // move playerbar
                playerPaddle.Move(-1, deltaTime);
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) // move playerbar
                playerPaddle.Move(1, deltaTime);

            if (ball.position.x > otherPaddle.position.x + otherPaddle.width / 2)
                otherPaddle.Move(1, deltaTime);
            else if (ball.position.x < otherPaddle.position.x - otherPaddle.width / 2)
                otherPaddle.Move(-1, deltaTime);

            if (ball.position.x > playerPaddle.position.x + playerPaddle.width / 2)
                playerPaddle.Move(1, deltaTime);
            else if (ball.position.x < playerPaddle.position.x - playerPaddle.width / 2)
                playerPaddle.Move(-1, deltaTime);

            ball.Move(deltaTime);

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
            text.RenderText(std::to_string(playerScore), -0.51f, -0.03f, 0.001f);
            text.RenderText(std::to_string(computerScore), 0.51f, -0.03f, 0.001f);
        }
        else if (currentGameState == GameState::START)
        {
            renderStartScreen();
        }
        else if (currentGameState == GameState::PAUSED)
        {
            renderPauseScreen();
            text.RenderText(std::to_string(playerScore), -0.51f, -0.03f, 0.001f);
            text.RenderText(std::to_string(computerScore), 0.51f, -0.03f, 0.001f);
        }

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
                    currentTime = std::chrono::high_resolution_clock::now();
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

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shaderProgram);
    glDeleteProgram(textShaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}