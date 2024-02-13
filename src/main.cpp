#include "defines.hpp"

#include "Window.hpp"
#include "pong.hpp"



int main()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    {
        Window window;
        GLFWwindow* winHandle = window.getWindowHandle();
        Pong pong(winHandle);
        glfwSetWindowUserPointer(winHandle, &pong);

        ShaderManager &sm = ShaderManager::getInstance();
        while (!glfwWindowShouldClose(winHandle))
        {
            glClear(GL_COLOR_BUFFER_BIT);

            pong.GameLoop();
            pong.Render();
            pong.PlayerInput();

            glfwSwapBuffers(winHandle);
            glfwPollEvents();

            sm.reloadShaderIfFileChange();
        }
    }

    // glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}