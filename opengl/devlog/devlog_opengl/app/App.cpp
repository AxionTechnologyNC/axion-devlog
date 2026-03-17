#include "pch.h"
#include "App.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

App::App(int width, int height, std::string title)
    : width_(width),
    height_(height),
    title_(std::move(title))
{}

App::~App()
{
    if (window_ != nullptr)
    {
        glfwDestroyWindow(window_);
        window_ = nullptr;
    }

    glfwTerminate();
}

bool App::init()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window_ = glfwCreateWindow(width_, height_, title_.c_str(), nullptr, nullptr);
    if (window_ == nullptr)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window_);
    glfwSetWindowUserPointer(window_, this);
    glfwSetFramebufferSizeCallback(window_, framebufferSizeCallback);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cerr << "Failed to initialize GLAD\n";
        return false;
    }

    glViewport(0, 0, width_, height_);

    printContextInfo();

    return true;
}

bool App::isRunning() const
{
    return window_ != nullptr && !glfwWindowShouldClose(window_);
}

void App::beginFrame()
{
    processSystemInput();

    glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void App::endFrame()
{
    glfwSwapBuffers(window_);
    glfwPollEvents();
}

float App::aspectRatio() const
{
    if (height_ == 0)
        return 1.0f;

    return static_cast<float>(width_) / static_cast<float>(height_);
}

float App::time() const
{
    return static_cast<float>(glfwGetTime());
}

int App::width() const
{
    return width_;
}

int App::height() const
{
    return height_;
}

GLFWwindow* App::nativeWindow() const
{
    return window_;
}

void App::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

    App* app = static_cast<App*>(glfwGetWindowUserPointer(window));
    if (app != nullptr)
    {
        app->width_ = width;
        app->height_ = height;
    }
}

void App::processSystemInput()
{
    if (window_ == nullptr)
        return;

    if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window_, true);
    }
}

void App::printContextInfo() const
{
    const GLubyte* vendor = glGetString(GL_VENDOR);
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    const GLubyte* glsl = glGetString(GL_SHADING_LANGUAGE_VERSION);

    std::cout << "OpenGL context initialized\n";
    std::cout << "Vendor   : " << (vendor ? reinterpret_cast<const char*>(vendor) : "unknown") << '\n';
    std::cout << "Renderer : " << (renderer ? reinterpret_cast<const char*>(renderer) : "unknown") << '\n';
    std::cout << "Version  : " << (version ? reinterpret_cast<const char*>(version) : "unknown") << '\n';
    std::cout << "GLSL     : " << (glsl ? reinterpret_cast<const char*>(glsl) : "unknown") << '\n';
}