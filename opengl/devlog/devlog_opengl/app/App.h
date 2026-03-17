#pragma once

#include <string>

struct GLFWwindow;

class App
{
public:
    App(int width, int height, std::string title);
    ~App();

    App(const App&) = delete;
    App& operator=(const App&) = delete;

    App(App&&) = delete;
    App& operator=(App&&) = delete;

    bool init();

    bool isRunning() const;

    void beginFrame();
    void endFrame();

    float aspectRatio() const;
    float time() const;

    int width() const;
    int height() const;

    GLFWwindow* nativeWindow() const;

private:
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

    void processSystemInput();
    void printContextInfo() const;

private:
    GLFWwindow* window_ = nullptr;

    int width_ = 0;
    int height_ = 0;
    std::string title_;
};