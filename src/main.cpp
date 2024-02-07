#include "defines.hpp"

// #include "paddle.hpp"
// #include "ball.hpp"
// #include "text.hpp"
#include "pong.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
//   static auto &shaderManager = ShaderManager::getInstance();
// shaderManager.createShaderProgram("BasicShader", { {GL_VERTEX_SHADER, "path/to/vertex_shader.glsl"}, {GL_FRAGMENT_SHADER, "path/to/fragment_shader.glsl"} });

void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

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

int main()
{
// auto &shaderManager = ShaderManager::getInstance();
  
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
    glClearColor(0.1f, 0.2f, 0.2f, 1.0f);

    // ShaderManager shaderManager;
    // shaderManager.getInstance();

    // std::shared_ptr<ShaderProgram> shaderProgram = shaderManager.getShaderProgram(
    //     {{GL_VERTEX_SHADER, "path/to/vertexShader.glsl"},
    //      {GL_FRAGMENT_SHADER, "path/to/fragmentShader.glsl"}});
    {
        Pong pong(window);

        while (!glfwWindowShouldClose(window))
        {

            glClear(GL_COLOR_BUFFER_BIT);

            pong.Render();
            pong.GameLoop();
            pong.PlayerInput();

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}