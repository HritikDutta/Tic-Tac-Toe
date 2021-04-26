#include "application.h"

#include <iostream>
#include <string>
#include <stb/stb_image.h>
#include "platform/fileio.h"
#include "universal/types.h"
#include "engine/gldebug.h"
#include "engine/ui.h"

int Application::activeApps = 0;

bool Application::Initialize()
{
    if (!activeApps)
    {
        if (!glfwInit())
            return false;
    }

    return true;
}

void Application::DoNothing(Application*) {}

void Application::CursorPositionCallback(GLFWwindow* window, f64 xpos, f64 ypos)
{
    Application* app = (Application*) glfwGetWindowUserPointer(window);
    app->mouseX = xpos / app->screenWidth  * app->refScreenWidth;
    app->mouseY = ypos / app->screenHeight * app->refScreenHeight;
}

void Application::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    Application* app = (Application*) glfwGetWindowUserPointer(window);

    app->screenWidth  = width;
    app->screenHeight = height;

    f32 aspectRatio = (f32) width / (f32) height;
    app->refScreenWidth = aspectRatio * app->refScreenHeight;

    glViewport(0, 0, width, height);
}

Application::Application(const char title[], int width, int height, bool fullscreen, bool debug)
{
    if (!Initialize())
        return;

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, (int) debug);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWmonitor* monitor = fullscreen ? glfwGetPrimaryMonitor() : NULL;
    window = glfwCreateWindow(width, height, title, monitor, NULL);
    if (!window)
        return;

    refScreenWidth  = screenWidth  = width;
    refScreenHeight = screenHeight = height;
    vsyncOn = false;

    onInit = onUpdate = onRender = DoNothing;

    glfwMakeContextCurrent(window);

    glfwSetWindowUserPointer(window, this);
    glfwSetCursorPosCallback(window, CursorPositionCallback);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
        return;

    if (!gladLoadGL())
        return;

    glEnable(GL_MULTISAMPLE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_DEPTH_TEST);

    if (debug)
    {
        // Check if debug output works
        int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
        {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(GLDebugOutput, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE,
                                  0, nullptr, GL_TRUE);

            std::cout << "[OpenGL] Ready to debug..." << std::endl;
        }
    }

    activeApps++;
}

Application::~Application()
{
    activeApps--;

    if (activeApps <= 0)
        glfwTerminate();
}

void Application::Run()
{
    UI::Init();

    onInit(this);

    f64 prevTime = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        time = glfwGetTime();
        deltaTime = time - prevTime;

        onUpdate(this);

        onRender(this);

        glfwSwapBuffers(window);
        glfwPollEvents();

        prevTime = time;
    }
}

void Application::Exit()
{
    glfwSetWindowShouldClose(window, 1);
}

bool Application::GetKey(int keyCode)
{
    return glfwGetKey(window, keyCode) == GLFW_PRESS;
}

bool Application::GetKeyDown(int keyCode)
{
    static u64 keyFlags[6] = { 0 };

    bool isPressed  = GetKey(keyCode);
    int keyIdx      = keyCode / 64;
    int keyFlagBit  = keyCode % 64;
    bool wasPressed = keyFlags[keyIdx] & (1 << keyFlagBit);

    if (isPressed)
    {
        if (!wasPressed)
            keyFlags[keyIdx] |= 1 << keyFlagBit;
    }
    else
    {
        keyFlags[keyIdx] &= ~(1 << keyFlagBit);
    }

    return isPressed && !wasPressed;
}

bool Application::GetKeyUp(int keyCode)
{
    return glfwGetKey(window, keyCode) == GLFW_RELEASE;
}

bool Application::GetMouseButton(int mouseButton)
{
    return glfwGetMouseButton(window, mouseButton) == GLFW_PRESS;
}

bool Application::GetMouseButtonDown(int mouseButton)
{
    static u8 mbFlags = 0;

    bool isPressed  = GetMouseButton(mouseButton);
    bool wasPressed = mbFlags & (1 << mouseButton);

    if (isPressed)
    {
        if (!wasPressed)
            mbFlags |= 1 << mouseButton;
    }
    else
    {
        mbFlags &= ~(1 << mouseButton);
    }

    return isPressed && !wasPressed;
}

bool Application::GetMouseButtonUp(int mouseButton)
{
    return glfwGetMouseButton(window, mouseButton) == GLFW_RELEASE;
}

void Application::SetWindowIcon(const char filepath[])
{
    GLFWimage icon;
    int channels;

    icon.pixels = stbi_load(filepath, &icon.width, &icon.height, &channels, 4);
    if (!icon.pixels)
    {
        std::cout << "Failed to load icon.\n";
        return;
    }

    glfwSetWindowIcon(window, 1, &icon);
    stbi_image_free(icon.pixels);
}

void Application::SetReferenceResolution(int resolution)
{
    f32 aspectRatio = (f32) screenWidth / (f32) screenHeight;

    refScreenHeight = resolution;
    refScreenWidth  = aspectRatio * resolution;
}

void Application::SetVsync(bool value)
{
    glfwSwapInterval((int) value);
}