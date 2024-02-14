#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_

#include "defines.hpp"

class Pong;



class Window
{
private:
    GLFWwindow *window;

public:
    Window();;
    ~Window()
    {
        glfwDestroyWindow(window);
    };
    void makeContextCurrent()
    {
        glfwMakeContextCurrent(window);
    };
    GLFWwindow *getWindowHandle() { return window; };
};

#endif //_WINDOW_HPP_