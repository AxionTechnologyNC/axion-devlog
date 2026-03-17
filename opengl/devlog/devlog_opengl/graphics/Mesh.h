#pragma once
#include <cstddef>
#include <glad/glad.h>

enum class VertexLayout
{
    Position3,
    Position3Color3,
    Position3Color3Tex2
};

class Mesh {
public:
    Mesh() = default;
    ~Mesh();

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    static Mesh create(const float* vertices,
        size_t verticesBytes,
        const unsigned int* indices,
        size_t indicesBytes,
        GLsizei indexCount,
        VertexLayout layout);

    bool isValid() const;
    void bind() const;
    void draw() const;

private:
    void destroy();

private:
    GLuint vao_ = 0;
    GLuint vbo_ = 0;
    GLuint ebo_ = 0;
    GLsizei indexCount_ = 0;
};