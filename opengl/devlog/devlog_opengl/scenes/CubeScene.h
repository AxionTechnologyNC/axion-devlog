#pragma once

#include "IScene.h"

#include "graphics/Shader.h"
#include "graphics/Mesh.h"
#include "graphics/Texture2D.h"
#include "camera/FlyCamera.h"
#include "ui/HudTextRenderer.h"

#include <glm/mat4x4.hpp>
#include <unordered_set>

class CubeScene final : public IScene
{
public:
    CubeScene() = default;
    ~CubeScene() override = default;

    bool init() override;
    void shutdown() override;

    void handleInput(GLFWwindow* window, float deltaTime) override;
    void onMouseMove(double xpos, double ypos) override;
    void onMouseScroll(double xoffset, double yoffset) override;
    void onKeyEvent(int key, int scancode, int action, int mods);

    void update(float time, float deltaTime, float aspectRatio) override;
    void render() override;
    void onResize(int width, int height) override;

private:
    static constexpr int kTextureUnit = 0;

    Shader shader_;
    Mesh cubeMesh_;
    Texture2D texture_;

    FlyCamera camera_{ glm::vec3(0.0f, 0.0f, 3.0f) };

    glm::mat4 model_{ 1.0f };
    glm::mat4 view_{ 1.0f };
    glm::mat4 proj_{ 1.0f };

    double lastX_ = 0.0;
    double lastY_ = 0.0;
    bool firstMouse_ = true;

    int framebufferWidth_ = 0;
    int framebufferHeight_ = 0;

    HudTextRenderer hudText_;
    std::unordered_set<int> pressedKeys_;
    int m_lastPressedKey = -1;

    bool initialized_ = false;
};