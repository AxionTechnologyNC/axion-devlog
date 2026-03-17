#include "pch.h"
#include "app/App.h"

#include "utils/FileTools.h"
#include "graphics/Shader.h"
#include "graphics/Mesh.h"
#include "graphics/Texture2D.h"

#include "glm/glm.hpp"
#include "glm/fwd.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

int main()
{
    App app(1280, 720, "Axion Devlog - OpenGL");
    if (!app.init()) return -1;


    const std::string vertexSource = readFile("../../assets/shaders/cubeVert.glsl");
    const std::string fragmentSource = readFile("../../assets/shaders/cubeFrag.glsl");

    Shader shader(vertexSource.c_str(), fragmentSource.c_str());
    if (!shader.isValid()) return -1;

    // -------------------- geometry --------------------
    auto vertices = loadVector<float>("../../assets/geometry/cube.txt");
    auto indices = loadVector<unsigned int>("../../assets/geometry/cube_indices.txt");

    Mesh cube = Mesh::create(
        vertices.data(),
        vertices.size() * sizeof(float),
        indices.data(),
        indices.size() * sizeof(unsigned int),
        static_cast<GLsizei>(indices.size()),
        VertexLayout::Position3Color3Tex2);

    Texture2D tex("../../assets/textures/axion_logo.png");
    
    if (!tex.isValid()) return -1;

    glEnable(GL_DEPTH_TEST);

    while (app.isRunning())
    {   
        app.beginFrame();
        
        int fbW, fbH;
        glfwGetFramebufferSize(app.nativeWindow(), &fbW, &fbH);
        float aspect = (fbH == 0) ? 1.0f : (float)app.width() / (float)app.height();

        glm::mat4 model(1.0f);
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.3f, 1.0f, 0.2f));

        glm::mat4 view(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        glm::mat4 proj = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

        shader.use();
        tex.bind(0);
        shader.setMat4("uModel", model);
        shader.setMat4("uView", view);
        shader.setMat4("uProj", proj);

        cube.draw();
        app.endFrame();
    }

	return 0;
}