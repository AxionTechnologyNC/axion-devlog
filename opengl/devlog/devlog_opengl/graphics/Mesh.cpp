#include "pch.h"
#include "Mesh.h"

Mesh::~Mesh()
{
    destroy();
}

Mesh::Mesh(Mesh&& other) noexcept
    : vao_(other.vao_),
    vbo_(other.vbo_),
    ebo_(other.ebo_),
    indexCount_(other.indexCount_)
{
    other.vao_ = 0;
    other.vbo_ = 0;
    other.ebo_ = 0;
    other.indexCount_ = 0;
}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
    if (this != &other) {
        destroy();

        vao_ = other.vao_;
        vbo_ = other.vbo_;
        ebo_ = other.ebo_;
        indexCount_ = other.indexCount_;

        other.vao_ = 0;
        other.vbo_ = 0;
        other.ebo_ = 0;
        other.indexCount_ = 0;
    }

    return *this;
}

Mesh Mesh::create(const float* vertices,
    size_t verticesBytes,
    const unsigned int* indices,
    size_t indicesBytes,
    GLsizei indexCount,
    VertexLayout layout)
{
    Mesh mesh;
    mesh.indexCount_ = indexCount;

    GLsizei stride = 0;
    bool hasColor = false;
    bool hasTexCoords = false;

    switch (layout)
    {
    case VertexLayout::Position3:
        stride = 3 * static_cast<GLsizei>(sizeof(float));
        break;

    case VertexLayout::Position3Color3:
        stride = 6 * static_cast<GLsizei>(sizeof(float));
        hasColor = true;
        break;

    case VertexLayout::Position3Color3Tex2:
        stride = 8 * static_cast<GLsizei>(sizeof(float));
        hasColor = true;
        hasTexCoords = true;
        break;

    default:
        std::cerr << "Unsupported VertexLayout\n";
        return mesh;
    }

    glGenVertexArrays(1, &mesh.vao_);
    glGenBuffers(1, &mesh.vbo_);
    glGenBuffers(1, &mesh.ebo_);

    glBindVertexArray(mesh.vao_);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo_);
    glBufferData(GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(verticesBytes),
        vertices,
        GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(indicesBytes),
        indices,
        GL_STATIC_DRAW);

    std::size_t offset = 0;

    // location 0 -> position.xyz
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        stride,
        reinterpret_cast<const void*>(static_cast<std::uintptr_t>(offset)));
    glEnableVertexAttribArray(0);
    offset += 3 * sizeof(float);

    if (hasColor) {
        // location 1 -> color.rgb
        glVertexAttribPointer(
            1,
            3,
            GL_FLOAT,
            GL_FALSE,
            stride,
            reinterpret_cast<const void*>(static_cast<std::uintptr_t>(offset)));
        glEnableVertexAttribArray(1);
        offset += 3 * sizeof(float);
    }

    if (hasTexCoords) {
        // location 2 -> texcoord.uv
        glVertexAttribPointer(
            2,
            2,
            GL_FLOAT,
            GL_FALSE,
            stride,
            reinterpret_cast<const void*>(static_cast<std::uintptr_t>(offset)));
        glEnableVertexAttribArray(2);
    }

    glBindVertexArray(0);

    return mesh;
}

bool Mesh::isValid() const
{
    return vao_ != 0 && vbo_ != 0 && ebo_ != 0 && indexCount_ > 0;
}

void Mesh::bind() const
{
    glBindVertexArray(vao_);
}

void Mesh::draw() const
{
    if (!isValid()) {
        return;
    }

    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, indexCount_, GL_UNSIGNED_INT, nullptr);
}

void Mesh::destroy()
{
    if (ebo_ != 0) {
        glDeleteBuffers(1, &ebo_);
        ebo_ = 0;
    }

    if (vbo_ != 0) {
        glDeleteBuffers(1, &vbo_);
        vbo_ = 0;
    }

    if (vao_ != 0) {
        glDeleteVertexArrays(1, &vao_);
        vao_ = 0;
    }

    indexCount_ = 0;
}