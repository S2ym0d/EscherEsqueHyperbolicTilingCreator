#include "Renderer.h"
#include "TilingSettings.h"
#include "Image.h"
#include "HyperbolicComputations.h"
#include "OPENGLInitializer.h"
#include <iostream>
#include <thread>
#include <mutex>
#include "WindowSettings.h"
#include <conio.h>

bool inputGoing;
std::mutex inputMutex;

const std::string DefaultTilingSettingsPath = "tilingSettings.txt";
const std::string DefaultSaveImagePath = "result.png";

std::string GetSettingsPath()
{
    std::string path;

    std::cout << "Provide the path to tiling settings path.\n";
    std::cout << "No path or without txt extension will result in using default path: " << DefaultTilingSettingsPath << std::endl;
    std::getline(std::cin, path);

    return path;
}

std::string GetPNGImagePath()
{
    std::string path;

    std::cout << "Provide the path to png image file.\n";
    std::cout << "No path or without png extension will result in using default path: " << DefaultSaveImagePath << std::endl;
    std::getline(std::cin, path);

    return path;
}

void InputManager(void (*inputFnc)())
{
    std::thread inputThread;
    inputMutex.lock();
    inputGoing = true;
    inputMutex.unlock();

    inputThread = std::thread(*inputFnc);

    while (true)
    {
        glfwPollEvents();

        inputMutex.lock();
        if (!inputGoing)
        {
            inputMutex.unlock();
            break;
        }
        inputMutex.unlock();
    }

    inputThread.join();
}

std::string tilingSettingsPath;

void InputSettingsPath()
{
    auto tsp = GetSettingsPath();

    if (tsp.size() == 0)
    {
        std::cout << "Empty settings path." << std::endl;
        tsp = DefaultTilingSettingsPath;
    }

    if (tsp.find(".txt") == std::string::npos)
    {
        std::cout << "No txt extension." << std::endl;
        tsp = DefaultTilingSettingsPath;
    }

    inputMutex.lock();
    tilingSettingsPath = tsp;
    inputGoing = false;
    inputMutex.unlock();
}

std::string saveImagePath;

void InputSaveImagePath()
{
    auto sip = GetPNGImagePath();

    if (sip.size() == 0)
    {
        std::cout << "Empty result image path." << std::endl;
        sip = DefaultSaveImagePath;
    }

    if (sip.find(".png") == std::string::npos)
    {
        std::cout << "No png extension." << std::endl;
        sip = DefaultSaveImagePath;
    }

    inputMutex.lock();
    saveImagePath = sip;
    inputGoing = false;
    inputMutex.unlock();
}

int main()
{
    auto windowSettings = LoadWindowSettings();

    OPENGLWindow window(windowSettings.width, windowSettings.height, WindowSettings::windowName);
    
    OPENGLInitializer::OPENGLInitializatin(window);

    std::cout << "Window size: " << window.GetWidth() << " x " << window.GetHeight() << std::endl;

    InputManager(InputSettingsPath);

    inputMutex.lock();
    auto tilingSettingsLoadingResult = LoadTilingSettings(tilingSettingsPath);
    inputMutex.unlock();

    if (!tilingSettingsLoadingResult.second)
    {
        glfwSetWindowShouldClose(window.GetWindow(), GLFW_TRUE);
        glfwTerminate();

        std::cout << "Press Enter to exit.";
        _getch();

        return 0;
    }

    auto& tilingSettings = tilingSettingsLoadingResult.first;

    GlobalUniforms globalUniforms;

    globalUniforms.windowSizes = { window.GetWidth(), window.GetHeight() };
    globalUniforms.separatingSides = tilingSettings.separatingSide;
    globalUniforms.sourceImageTransformations = { static_cast<float>(tilingSettings.sourceImageLocalRotation), tilingSettings.sourceImageTranslation, static_cast<float>(tilingSettings.sourceImageGlobalRotation) };
    globalUniforms.sourceImageModel = tilingSettings.sourceImageModel;
    globalUniforms.tilingImageTransformations = { static_cast<float>(tilingSettings.tilingImageLocalRotation), tilingSettings.tilingImageTranslation, static_cast<float>(tilingSettings.tilingImageGlobalRotation) };
    globalUniforms.tilingImageModel = tilingSettings.tilingImageModel;

    auto fillGlobalUniformsResult = FillGlobalUniforms(tilingSettings, globalUniforms);

    if (!fillGlobalUniformsResult.second)
    {
        glfwSetWindowShouldClose(window.GetWindow(), GLFW_TRUE);
        glfwTerminate();

        std::cout << "Press Enter to exit.";
        _getch();

        return 0;
    }

    auto& fundamentalTriangle = fillGlobalUniformsResult.first;

    std::cout << "\n";
    std::cout << "Vertex p: " << (fundamentalTriangle.pqrVertices.at(0).IsIdeal() ? fundamentalTriangle.pqrVertices.at(0).ToString() : fundamentalTriangle.pqrVertices.at(0).ToHyperboloidModel().ToString()) << "\n";
    std::cout << "Vertex q: " << (fundamentalTriangle.pqrVertices.at(1).IsIdeal() ? fundamentalTriangle.pqrVertices.at(1).ToString() : fundamentalTriangle.pqrVertices.at(1).ToHyperboloidModel().ToString()) << "\n";
    std::cout << "Vertex r: " << (fundamentalTriangle.pqrVertices.at(2).IsIdeal() ? fundamentalTriangle.pqrVertices.at(2).ToString() : fundamentalTriangle.pqrVertices.at(2).ToHyperboloidModel().ToString()) << "\n";
    std::cout << "\n";
    std::cout << "Incenter: " << fundamentalTriangle.incenter.ToHyperboloidModel().ToString() << "\n";
    std::cout << std::endl;

    const auto& imamge0Path = tilingSettings.evenImageFilePath, imamge1Path = tilingSettings.oddEvenImageTiling ? tilingSettings.oddImageFilePath : tilingSettings.evenImageFilePath;

    Image image0(imamge0Path, window.GetWidth(), window.GetHeight()), image1(imamge1Path, window.GetWidth(), window.GetHeight());

    if (!image0.IsImageProper())
    {
        glfwSetWindowShouldClose(window.GetWindow(), GLFW_TRUE);
        glfwTerminate();

        std::cout << "Press Enter to exit.";
        _getch();

        return 0;
    }

    if (!image1.IsImageProper())
    {
        glfwSetWindowShouldClose(window.GetWindow(), GLFW_TRUE);
        glfwTerminate();

        std::cout << "Press Enter to exit.";
        _getch();

        return 0;
    }

    Renderer rend;

    rend.InitializeShaders(globalUniforms);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSwapInterval(1);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    window.SwapBuffers();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    window.SwapBuffers();

    rend.RenderTiling(globalUniforms, { image0,  image1 });

    Image result(window.GetWidth(), window.GetHeight(), 4);

    glReadPixels(0, 0, window.GetWidth(), window.GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, result.Data().data());
    window.SwapBuffers(); 
    
    std::cout << "Saving result image." << std::endl;

    InputManager(InputSaveImagePath);

    inputMutex.lock();
    result.Save(saveImagePath);
    inputMutex.unlock();

    std::cout << "End" << std::endl;

    std::cout << "Press Enter to exit.";
    _getch();

    glfwSetWindowShouldClose(window.GetWindow(), GLFW_TRUE);
    glfwTerminate();
    return 0;
}