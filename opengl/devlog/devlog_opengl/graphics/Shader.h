#pragma once

#include <glad/glad.h>
#include <glm/fwd.hpp>

class Shader
{
public:
    Shader() = default;
    Shader(const char* vertexSource, const char* fragmentSource);
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    bool isValid() const;

    void use() const;

    void setInt(const char* name, int value) const;
    void setFloat(const char* name, float value) const;
    void setMat4(const char* name, const glm::mat4& value) const;

private:
    static GLuint compileShader(GLenum type, const char* source);
    static GLuint createProgram(const char* vertexSource, const char* fragmentSource);

    GLint getUniformLocation(const char* name) const;
    void destroy();

private:
    GLuint id_ = 0;
};