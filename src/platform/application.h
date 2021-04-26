#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "universal/types.h"

#define KEY(x) GLFW_KEY_##x
#define MOUSE(x) GLFW_MOUSE_BUTTON_##x

struct Application
{
    Application(const char title[], int width, int height, bool fullscreen = false, bool debug = false);
    ~Application();

    void Run();
    void Exit();

    bool GetKey(int keyCode);
    bool GetKeyDown(int keyCode);
    bool GetKeyUp(int keyCode);

    bool GetMouseButton(int mouseButton);
    bool GetMouseButtonDown(int mouseButton);
    bool GetMouseButtonUp(int mouseButton);

    void SetWindowIcon(const char filepath[]);
    void SetReferenceResolution(int resolution);
    void SetVsync(bool value);

    GLFWwindow* window;
    s32 screenWidth, screenHeight;
    s32 refScreenWidth, refScreenHeight;
    bool vsyncOn;

    f64 mouseX, mouseY;
    f64 deltaTime;
    f64 time;

    void (*onInit)(Application* app);
    void (*onUpdate)(Application* app);
    void (*onRender)(Application* app);

    static int activeApps;
    static bool Initialize();
    static void DoNothing(Application*);
    static void CursorPositionCallback(GLFWwindow* window, f64 xpos, f64 ypos);
    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
};