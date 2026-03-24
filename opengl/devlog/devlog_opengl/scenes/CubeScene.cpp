#include "pch.h"
#include "CubeScene.h"

#include "utils/FileTools.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

bool CubeScene::init()
{
    const std::string vertexSource = readFile("../../assets/shaders/cubeVert.glsl");
    const std::string fragmentSource = readFile("../../assets/shaders/cubeFrag.glsl");

    if (vertexSource.empty() || fragmentSource.empty()) {
        std::cerr << "Failed to read shader source files for CubeScene\n";
        return false;
    }

    shader_ = Shader(vertexSource.c_str(), fragmentSource.c_str());
    if (!shader_.isValid()) {
        std::cerr << "Failed to create shader program for CubeScene\n";
        return false;
    }

    const std::vector<float> vertices = 
        loadVector<float>("../../assets/geometry/cube.txt");
    const std::vector<unsigned int> indices = 
        loadVector<unsigned int>("../../assets/geometry/cube_indices.txt");

    if (vertices.empty() || indices.empty()) {
        std::cerr << "Failed to load cube geometry data\n";
        return false;
    }

    cubeMesh_ = Mesh::create(
        vertices.data(),
        vertices.size() * sizeof(float),
        indices.data(),
        indices.size() * sizeof(unsigned int),
        static_cast<GLsizei>(indices.size()),
        VertexLayout::Position3Color3Tex2);

    if (!cubeMesh_.isValid()) {
        std::cerr << "Failed to create cube mesh\n";
        return false;
    }

    texture_ = Texture2D("../../assets/textures/bioforge_sentinel.png");
    if (!texture_.isValid()) {
        std::cerr << "Failed to load cube texture\n";
        return false;
    }

    glEnable(GL_DEPTH_TEST);

    shader_.use();
    shader_.setInt("uTex", kTextureUnit);

    initialized_ = true;
    return true;
}

void CubeScene::shutdown()
{
    if (!initialized_) return;

    texture_ = Texture2D();
    cubeMesh_ = Mesh();
    shader_ = Shader();

    initialized_ = false;
}

void CubeScene::handleInput(GLFWwindow* window, float deltaTime)
{
    if (!initialized_ || window == nullptr)
        return;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera_.move(CameraMoveDirection::Forward, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera_.move(CameraMoveDirection::Backward, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera_.move(CameraMoveDirection::Left, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera_.move(CameraMoveDirection::Right, deltaTime);

    // Optional: vertical flight
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera_.move(CameraMoveDirection::Down, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera_.move(CameraMoveDirection::Up, deltaTime);

    // Rotate the camera using the keyboard (like fallback to the mouse)
    constexpr float kKeyboardLookSpeed = 450.0f;

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        camera_.rotate(-kKeyboardLookSpeed * deltaTime, 0.0f);

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        camera_.rotate(kKeyboardLookSpeed * deltaTime, 0.0f);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera_.rotate(0.0f, kKeyboardLookSpeed * deltaTime);

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera_.rotate(0.0f, -kKeyboardLookSpeed * deltaTime);
}

void CubeScene::onMouseMove(double xpos, double ypos)
{
    if (!initialized_)
        return;

    if (firstMouse_) {
        lastX_ = xpos;
        lastY_ = ypos;
        firstMouse_ = false;
    }

    const float xoffset = static_cast<float>(xpos - lastX_);
    const float yoffset = static_cast<float>(lastY_ - ypos);

    lastX_ = xpos;
    lastY_ = ypos;

    camera_.rotate(xoffset, yoffset);
}

void CubeScene::onMouseScroll(double xoffset, double yoffset)
{
    (void)xoffset;

    if (!initialized_)
        return;

    camera_.zoom(static_cast<float>(yoffset));
}

void CubeScene::update(float time, float deltaTime, float aspectRatio)
{
    (void)deltaTime;

    if (!initialized_) return;

    view_ = camera_.getViewMatrix();

    proj_ = glm::perspective(
        glm::radians(camera_.zoomDegrees()),
        aspectRatio,
        0.1f,
        100.0f);
}

void CubeScene::render()
{
    if (!initialized_) return;

    shader_.use();
    texture_.bind(kTextureUnit);

    shader_.setMat4("uModel", model_);
    shader_.setMat4("uView", view_);
    shader_.setMat4("uProj", proj_);

    cubeMesh_.draw();

    glUseProgram(0); // for legacy HUD    
    hudText_.render(framebufferWidth_, framebufferHeight_);
}

void CubeScene::onResize(int width, int height)
{
    framebufferWidth_ = width;
    framebufferHeight_ = height;
}

static const char* keyToString(int key)
{
    switch (key)
    {
    case GLFW_KEY_W: return "W";
    case GLFW_KEY_A: return "A";
    case GLFW_KEY_S: return "S";
    case GLFW_KEY_D: return "D";
    case GLFW_KEY_Q: return "Q";
    case GLFW_KEY_E: return "E";
    case GLFW_KEY_LEFT: return "LEFT";
    case GLFW_KEY_RIGHT: return "RIGHT";
    case GLFW_KEY_UP: return "UP";
    case GLFW_KEY_DOWN: return "DOWN";
    default: return "UNKNOWN";
    }
}

void CubeScene::onKeyEvent(int key, int scancode, int action, int mods)
{
    (void)scancode;
    (void)mods;

    if (action == GLFW_PRESS) {
        pressedKeys_.insert(key);
        m_lastPressedKey = key;
    }
    else if (action == GLFW_RELEASE) {
        pressedKeys_.erase(key);

        if (pressedKeys_.empty())
            m_lastPressedKey = -1;
    }

    if (!pressedKeys_.empty()) {
        int last = m_lastPressedKey;
        hudText_.setText(std::string("Current pressed key: ") + keyToString(last));

        // Color can be changed dynamically
        hudText_.setColor(glm::vec4(0.2f, 1.0f, 0.2f, 1.0f));
    }
    else {
        hudText_.setText("Current pressed key: None");
        hudText_.setColor(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
    }
}