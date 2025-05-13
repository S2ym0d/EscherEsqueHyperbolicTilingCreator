#pragma once
#include "OPENGLWindow.h"

struct OPENGLInitializer
{
	static bool OPENGLglfwInit();
	static bool OPENGLglfwWindowCreation(OPENGLWindow& window);
	static bool OPENGLglfwCurrentContextMake(OPENGLWindow& window);
	static bool OPENGLglewInit();
	static void OPENGLVersion();

	static bool OPENGLInitializatin(OPENGLWindow& window);
};

