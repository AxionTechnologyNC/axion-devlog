#include "pch.h"
#include "CubeScene.h"

#include "utils/FileTools.h"

#include <glad/glad.h>
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

    texture_ = Texture2D("../../assets/textures/axion_logo.png");
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

void CubeScene::update(float time, float aspectRatio)
{
    if (!initialized_) return;

    model_ = glm::mat4(1.0f);
    model_ = glm::rotate(model_, time, glm::vec3(0.3f, 1.0f, 0.2f));

    view_ = glm::mat4(1.0f);
    view_ = glm::translate(view_, glm::vec3(0.0f, 0.0f, -3.0f));

    proj_ = glm::perspective(
        glm::radians(45.0f),
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
}