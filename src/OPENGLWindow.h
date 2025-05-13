#pragma once
#include "OPENGLHeaderFiles.h"
#include <string>

class OPENGLWindow
{
private:
	GLFWwindow* window;
	int width;
	int height;
	std::string title;
	GLFWmonitor* monitor;
	GLFWwindow* share;
public:
	OPENGLWindow();
	OPENGLWindow(int width, int height);
	OPENGLWindow(int width, int height, std::string title);
	OPENGLWindow(int width, int height, std::string title,GLFWmonitor* monitor, GLFWwindow* share);

	bool CreateWindow();

	GLFWwindow* GetWindow() const;
	int GetWidth() const;
	int GetHeight() const;
	std::string GetTitle() const;

	int WindowShouldClose() const;
	void SwapBuffers();

	void DestroyWindow() const;
};

