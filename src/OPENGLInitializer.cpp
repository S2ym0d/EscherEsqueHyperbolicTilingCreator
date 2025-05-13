#include "OPENGLInitializer.h"
#include "OPENGLHeaderFiles.h"
#include <iostream>

bool OPENGLInitializer::OPENGLglfwInit()
{
    /* Initialize the library */
    if (!glfwInit())
        return false;

    return true;
}

bool OPENGLInitializer::OPENGLglfwWindowCreation(OPENGLWindow& window)
{
    return window.CreateWindow();
}

bool OPENGLInitializer::OPENGLglfwCurrentContextMake(OPENGLWindow& window)
{
    glfwMakeContextCurrent(window.GetWindow());

    return true;
}

bool OPENGLInitializer::OPENGLglewInit()
{
    if (glewInit() != GLEW_OK)
        return false;

    return true;
}

void OPENGLInitializer::OPENGLVersion()
{
    std::cout << glGetString(GL_VERSION) << std::endl;
}

bool OPENGLInitializer::OPENGLInitializatin(OPENGLWindow& window)
{
    if (!OPENGLglfwInit())
    {
        std::cout << "Error at glfw initialization." << std::endl;
        return false;
    }

    if (!OPENGLglfwWindowCreation(window))
    {
        std::cout << "Error at window creation." << std::endl;
        return false;
    }

    if (!OPENGLglfwCurrentContextMake(window))
    {
        std::cout << "Error at current context making." << std::endl;
        return false;
    }

    if (!OPENGLglewInit())
    {
        std::cout << "Error at glew initialization." << std::endl;
        return false;
    }

    OPENGLVersion();

    return true;
}
