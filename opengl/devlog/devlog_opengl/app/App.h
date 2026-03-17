#pragma once

#include <memory>
#include <string>

struct GLFWwindow;
class IScene;

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

    void setScene(std::unique_ptr<IScene> scene);

    int run();
    void close();

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
    void shutdownScene();

private:
    GLFWwindow* window_ = nullptr;

    int width_ = 0;
    int height_ = 0;
    std::string title_;

    std::unique_ptr<IScene> scene_;
};