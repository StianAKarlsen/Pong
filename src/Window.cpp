#include "defines.hpp"
#include "pong.hpp"

#include "Window.hpp"

static void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
};
static void windowRefreshCallback(GLFWwindow *window)
{
    Pong *pong = (Pong *)glfwGetWindowUserPointer(window);
    glClear(GL_COLOR_BUFFER_BIT);

    pong->GameLoop();
    pong->Render();
    pong->PlayerInput();

    glfwSwapBuffers(window);
};

static void windowWindowPosCallback(GLFWwindow *window, int x, int y)
{
    Pong *pong = (Pong *)glfwGetWindowUserPointer(window);
    glClear(GL_COLOR_BUFFER_BIT);

    pong->GameLoop();
    pong->Render();
    pong->PlayerInput();

    glfwSwapBuffers(window);
};

Window::Window()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
    // glfwWindowHint(GLFW_DECORATED, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(640, 480, "PONG", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
    }
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetWindowRefreshCallback(window, windowRefreshCallback);
    // glfwSetWindowFocusCallback(window, windowRefreshCallback);
    glfwSetWindowPosCallback(window, windowWindowPosCallback);
    glfwMakeContextCurrent(window);
    glClearColor(0.1f, 0.2f, 0.2f, 1.0f);
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
    }

    glfwSwapInterval(1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // glEnable(GL_DEPTH_TEST);
    // glPolygonMode(GL_BACK, GL_FILL);
};