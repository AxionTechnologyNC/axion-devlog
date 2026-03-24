#pragma once

struct GLFWwindow;

class IScene
{
public:
    virtual ~IScene() = default;

    virtual bool init() = 0;
    virtual void shutdown() {}

    virtual void handleInput(GLFWwindow* window, float deltaTime) {}
    virtual void onMouseMove(double xpos, double ypos) {}
    virtual void onMouseScroll(double xoffset, double yoffset) {}
    virtual void onKeyEvent(int key, int scancode, int action, int mods) {}

    virtual void update(float time, float deltaTime, float aspectRatio) = 0;
    virtual void render() = 0;

    virtual void onResize(int width, int height) {}
};