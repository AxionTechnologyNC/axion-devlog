#include "pch.h"
#include "Image.h"

#include <stb/stb_image.h>

Image::Image(const char* path, ImageLoadOptions options)
{
    stbi_set_flip_vertically_on_load(options.flipVertically ? 1 : 0);

    pixels_ = stbi_load(
        path,
        &width_,
        &height_,
        &channels_,
        options.desiredChannels);

    if (pixels_ != nullptr && options.desiredChannels != 0) {
        channels_ = options.desiredChannels;
    }
}

Image::~Image()
{
    destroy();
}

Image::Image(Image&& other) noexcept
    : pixels_(other.pixels_),
    width_(other.width_),
    height_(other.height_),
    channels_(other.channels_)
{
    other.pixels_ = nullptr;
    other.width_ = 0;
    other.height_ = 0;
    other.channels_ = 0;
}

Image& Image::operator=(Image&& other) noexcept
{
    if (this != &other) {
        destroy();

        pixels_ = other.pixels_;
        width_ = other.width_;
        height_ = other.height_;
        channels_ = other.channels_;

        other.pixels_ = nullptr;
        other.width_ = 0;
        other.height_ = 0;
        other.channels_ = 0;
    }

    return *this;
}

bool Image::isValid() const
{
    return pixels_ != nullptr;
}

const unsigned char* Image::data() const
{
    return pixels_;
}

int Image::width() const
{
    return width_;
}

int Image::height() const
{
    return height_;
}

int Image::channels() const
{
    return channels_;
}

void Image::destroy()
{
    if (pixels_ != nullptr) {
        stbi_image_free(pixels_);
        pixels_ = nullptr;
    }

    width_ = 0;
    height_ = 0;
    channels_ = 0;
}