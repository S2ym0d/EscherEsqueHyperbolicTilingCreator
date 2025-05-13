#pragma once
#include <string>

struct WindowSettings
{
	int width = 800;
	int height = 600;
	static std::string windowName;
};

WindowSettings LoadWindowSettings();

