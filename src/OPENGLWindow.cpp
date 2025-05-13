#include "OPENGLWindow.h"

OPENGLWindow::OPENGLWindow() : width(640), height(480), title("Hello World"), monitor(NULL), share(NULL), window(NULL)
{
}

OPENGLWindow::OPENGLWindow(int width, int height) : width(width), height(height), title(" "), monitor(NULL), share(NULL), window(NULL)
{
}

OPENGLWindow::OPENGLWindow(int width, int height, std::string title) : width(width), height(height), title(title), monitor(NULL), share(NULL), window(NULL)
{
}

OPENGLWindow::OPENGLWindow(int width, int height, std::string title, GLFWmonitor* monitor, GLFWwindow* share) : width(width), height(height), title(title), monitor(monitor), share(share), window(NULL)
{
}

bool OPENGLWindow::CreateWindow()
{
    if (window != NULL)
    {
        return false;
    }

    window = glfwCreateWindow(width, height, title.c_str(), monitor, share);
    if (!window)
    {
        glfwTerminate();
        return false;
    }

    glfwGetFramebufferSize(window, &width, &height);

    return true;
}

GLFWwindow* OPENGLWindow::GetWindow() const
{
    return window;
}

int OPENGLWindow::GetWidth() const
{
    return width;
}

int OPENGLWindow::GetHeight() const
{
    return height;
}

std::string OPENGLWindow::GetTitle() const
{
    return std::string(title);
}

int OPENGLWindow::WindowShouldClose() const
{
    return glfwWindowShouldClose(window);
}

void OPENGLWindow::SwapBuffers()
{
    glfwSwapBuffers(window);
}

void OPENGLWindow::DestroyWindow() const
{
    if (window == NULL) return;

    if (glfwWindowShouldClose(window)) return;

    glfwDestroyWindow(window);
}
