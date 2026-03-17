#include "pch.h"
#include "Texture2D.h"

#include "Image.h"

Texture2D::Texture2D(const char* path, bool flipVertically)
{
    Image image(path, ImageLoadOptions{flipVertically, 0});
    if (image.data() == nullptr) {
        std::cerr << "Failed to load texture image: " << path << '\n';
        return;
    }

    GLenum format = GL_RGB;
    GLenum internalFormat = GL_RGB8;

    switch (image.channels())
    {
    case 1:
        format = GL_RED;
        internalFormat = GL_R8;
        break;

    case 3:
        format = GL_RGB;
        internalFormat = GL_RGB8;
        break;

    case 4:
        format = GL_RGBA;
        internalFormat = GL_RGBA8;
        break;

    default:
        std::cerr << "Unsupported image channel count for texture: "
            << image.channels() << " (" << path << ")\n";
        return;
    }

    glGenTextures(1, &id_);
    glBindTexture(GL_TEXTURE_2D, id_);

    // axes S, T, R, equivalent to axes X, Y, Z
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        internalFormat,
        image.width(),
        image.height(),
        0,
        format,
        GL_UNSIGNED_BYTE,
        image.data());

    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2D::~Texture2D()
{
    destroy();
}

Texture2D::Texture2D(Texture2D&& other) noexcept
    : id_(other.id_)
{
    other.id_ = 0;
}

Texture2D& Texture2D::operator=(Texture2D&& other) noexcept
{
    if (this != &other) {
        destroy();

        id_ = other.id_;
        other.id_ = 0;
    }

    return *this;
}

bool Texture2D::isValid() const
{
    return id_ != 0;
}

void Texture2D::bind(unsigned int unit) const
{
    if (id_ == 0) return;

    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, id_);
}

void Texture2D::destroy()
{
    if (id_ != 0) {
        glDeleteTextures(1, &id_);
        id_ = 0;
    }
}