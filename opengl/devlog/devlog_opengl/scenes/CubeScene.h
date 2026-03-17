#pragma once

#include "IScene.h"

#include "graphics/Shader.h"
#include "graphics/Mesh.h"
#include "graphics/Texture2D.h"

#include <glm/mat4x4.hpp>

class CubeScene final : public IScene
{
public:
    CubeScene() = default;
    ~CubeScene() override = default;

    bool init() override;
    void shutdown() override;

    void update(float time, float aspectRatio) override;
    void render() override;

private:
    static constexpr int kTextureUnit = 0;

    Shader shader_;
    Mesh cubeMesh_;
    Texture2D texture_;

    glm::mat4 model_{ 1.0f };
    glm::mat4 view_{ 1.0f };
    glm::mat4 proj_{ 1.0f };

    bool initialized_ = false;
};