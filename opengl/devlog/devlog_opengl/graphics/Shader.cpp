#include "pch.h"
#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char* vertexSource, const char* fragmentSource)
    : id_(createProgram(vertexSource, fragmentSource))
{}

Shader::~Shader()
{
    destroy();
}

Shader::Shader(Shader&& other) noexcept
    : id_(other.id_)
{
    other.id_ = 0;
}

Shader& Shader::operator=(Shader&& other) noexcept
{
    if (this != &other)
    {
        destroy();

        id_ = other.id_;
        other.id_ = 0;
    }

    return *this;
}

bool Shader::isValid() const
{
    return id_ != 0;
}

void Shader::use() const
{
    if (id_ != 0)
    {
        glUseProgram(id_);
    }
}

void Shader::setInt(const char* name, int value) const
{
    const GLint location = getUniformLocation(name);
    if (location != -1)
    {
        glUniform1i(location, value);
    }
}

void Shader::setFloat(const char* name, float value) const
{
    const GLint location = getUniformLocation(name);
    if (location != -1)
    {
        glUniform1f(location, value);
    }
}

void Shader::setMat4(const char* name, const glm::mat4& value) const
{
    const GLint location = getUniformLocation(name);
    if (location != -1)
    {
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }
}

GLuint Shader::compileShader(GLenum type, const char* source)
{
    const GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (success == GL_FALSE)
    {
        GLint infoLogLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

        std::string infoLog;
        infoLog.resize(static_cast<size_t>(infoLogLength > 0 ? infoLogLength : 1));

        glGetShaderInfoLog(shader, infoLogLength, nullptr, &infoLog[0]);

        const char* shaderTypeName = "UNKNOWN";
        if (type == GL_VERTEX_SHADER) shaderTypeName = "VERTEX";
        else if (type == GL_FRAGMENT_SHADER) shaderTypeName = "FRAGMENT";

        std::cerr << "Failed to compile " << shaderTypeName << " shader: " << infoLog << '\n';

        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLuint Shader::createProgram(const char* vertexSource, const char* fragmentSource)
{
    const GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    if (vertexShader == 0)
    {
        return 0;
    }

    const GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
    if (fragmentShader == 0)
    {
        glDeleteShader(vertexShader);
        return 0;
    }

    const GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    if (success == GL_FALSE)
    {
        GLint infoLogLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

        std::string infoLog;
        infoLog.resize(static_cast<size_t>(infoLogLength > 0 ? infoLogLength : 1));

        glGetProgramInfoLog(program, infoLogLength, nullptr, &infoLog[0]);

        std::cerr << "Failed to link shader program: " << infoLog << '\n';

        glDeleteProgram(program);
        return 0;
    }

    return program;
}

GLint Shader::getUniformLocation(const char* name) const
{
    if (id_ == 0)
    {
        return -1;
    }

    const GLint location = glGetUniformLocation(id_, name);
    if (location == -1)
    {
        std::cerr << "Warning: uniform not found or unused: " << name << '\n';
    }

    return location;
}

void Shader::destroy()
{
    if (id_ != 0)
    {
        glDeleteProgram(id_);
        id_ = 0;
    }
}