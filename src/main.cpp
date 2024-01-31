#include "defines.hpp"

// #include "paddle.hpp"
// #include "ball.hpp"
// #include "text.hpp"
#include "pong.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

GLuint playerScore = 0, computerScore = 0;
GLuint VAO[TOTAL_VAO], VBO[TOTAL_VBO], textures[TOTAL_TEXTURES];
GLuint shaderProgram, textShaderProgram;
GLint modelPos, textModelPos;
bool enterKeyPressedOnce = false;


GLfloat fullScreenVertices[] = {
    -1.0f, 1.0f, 0.0f, 0.0f,
    -1.0f, -1.0f, 0.0f, 1.0f,
    1.0f, -1.0f, 1.0f, 1.0f,

    -1.0f, 1.0f, 0.0f, 0.0f,
    1.0f, -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 0.0f};

void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// void renderGame()
// {
//     glUseProgram(shaderProgram);

//     glActiveTexture(GL_TEXTURE0);
//     glBindTexture(GL_TEXTURE_2D, textures[0]);
//     glUniform1i(glGetUniformLocation(shaderProgram, "textureSampler"), 0);

//     glBindVertexArray(VAO[2]);
//     static const GLfloat pos[] = {0, 0};
//     glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), GL_TRUE);

//     glUniform2fv(modelPos, 1, pos);
//     glDrawArrays(GL_TRIANGLES, 0, 6);

//     glBindTexture(GL_TEXTURE_2D, 0);
//     glBindVertexArray(0);
// };

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

    // GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // glCompileShader(vertexShader);

    // GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    // glCompileShader(fragmentShader);

    // GLuint textFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // glShaderSource(textFragmentShader, 1, &textFragmentShaderSource, NULL);
    // glCompileShader(textFragmentShader);

    // shaderProgram = glCreateProgram();
    // glAttachShader(shaderProgram, vertexShader);
    // glAttachShader(shaderProgram, fragmentShader);
    // glLinkProgram(shaderProgram);

    // GLuint textShaderProgram = glCreateProgram();
    // glAttachShader(textShaderProgram, vertexShader);
    // glAttachShader(textShaderProgram, textFragmentShader);
    // glLinkProgram(textShaderProgram);

    // glDeleteShader(vertexShader);
    // glDeleteShader(fragmentShader);
    // glDeleteShader(textFragmentShader);

    // modelPos = glGetUniformLocation(shaderProgram, "modelPos");
    // textModelPos = glGetUniformLocation(textShaderProgram, "modelPos");

    // Text text(textShaderProgram);

    glGenVertexArrays(TOTAL_VAO, VAO);
    glGenBuffers(TOTAL_VBO, VBO);

    glGenTextures(TOTAL_TEXTURES, textures);

    // LoadTexture(backgroundImage_png, backgroundImage_png_len, textures[0]);
    // LoadTexture(pauseImage_png, pauseImage_png_len, textures[1]);
    LoadTexture(startImage_png, startImage_png_len, textures[2]);

    glBindVertexArray(VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fullScreenVertices), fullScreenVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *)(2 * sizeof(GLfloat)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Pong pong(window);

    glUseProgram(shaderProgram);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.1f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

            pong.Render();
            pong.GameLoop();
            pong.PlayerInput();

        // else if (currentGameState == GameState::START)
        // {
        //     renderStartScreen();
        // }
        // else if (currentGameState == GameState::PAUSED)
        // {
        //     // renderPauseScreen();
        //     // playerPaddle.render();
        //     // otherPaddle.render();
        //     // ball.render();
        //     pong.Render();
        //     // text.RenderText(std::to_string(playerScore), -0.51f, -0.03f, 0.001f);
        //     // text.RenderText(std::to_string(computerScore), 0.51f, -0.03f, 0.001f);


        // }



        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shaderProgram);
    glDeleteProgram(textShaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}