#pragma once

#include <glad/glad.h>

class Texture2D
{
public:
    Texture2D() = default;
    explicit Texture2D(const char* path, bool flipVertically = true);
    ~Texture2D();

    Texture2D(const Texture2D&) = delete;
    Texture2D& operator=(const Texture2D&) = delete;

    Texture2D(Texture2D&& other) noexcept;
    Texture2D& operator=(Texture2D&& other) noexcept;

    bool isValid() const;

    void bind(unsigned int unit = 0) const;

private:
    void destroy();

private:
    GLuint id_ = 0;
};