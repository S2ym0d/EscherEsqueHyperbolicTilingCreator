#include "WindowSettings.h"
#include <fstream>
#include <iostream>
#include "HelpfulStringFunctions.h"
#include <functional>
#include <array>

std::string WindowSettings::windowName = "Escher esque Hyperbolic Tiling Creator";

const std::string windowSettingFilePath = "res/settings/windowSettings.txt";

void LoadWidth(WindowSettings& settings, const std::string& line)
{
    try
    {
        int val{ std::stoi(line) };
        settings.width = val;
    }
    catch (std::invalid_argument const& ex)
    {
        std::cout << "Invalid argument: " << line << "as window width." << std::endl;
        return;
    }
    catch (std::out_of_range const& ex)
    {
        std::cout << "Argument out of range: " << line << "as window width." << std::endl;
        return;
    }
}

void LoadHeight(WindowSettings& settings, const std::string& line)
{
    try
    {
        int val{ std::stoi(line) };
        settings.height = val;
    }
    catch (std::invalid_argument const& ex)
    {
        std::cout << "Invalid argument: " << line << "as window height." << std::endl;
        return;
    }
    catch (std::out_of_range const& ex)
    {
        std::cout << "Argument out of range: " << line << "as window height." << std::endl;
        return;
    }
}

const std::array<std::pair<std::string, std::function<void(WindowSettings&, const std::string&)>>, 2> settingsNames =
{
    std::pair<std::string, std::function<void(WindowSettings&, const std::string&)>>({"width:", LoadWidth}),
    {"height:", LoadHeight}
};

WindowSettings LoadWindowSettings()
{
    std::ifstream settingsFile;

    settingsFile.open(windowSettingFilePath);

    if (!settingsFile.is_open())
    {
        std::cout << "Settings file " << windowSettingFilePath << " couldn't be opened." << std::endl;

        return WindowSettings();
    }

    WindowSettings settings;

    std::string line;

    while (std::getline(settingsFile, line))
    {
        line = SubstringBeforeFirstOccurrenceOfSeparator(line, "#");

        for (const auto& setting : settingsNames)
        {
            if (line.find(setting.first) != std::string::npos)
            {
                line = SubstringAfterFirstOccurrenceOfSeparator(line, setting.first);

                setting.second(settings, line);

                break;
            }
        }
    }

    settingsFile.close();

    return settings;
}