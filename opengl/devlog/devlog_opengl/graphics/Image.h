#pragma once

struct ImageLoadOptions
{
    bool flipVertically = true;
    int desiredChannels = 0; // 0 = keep original
};

class Image
{
public:
    Image() = default;
    explicit Image(const char* path, ImageLoadOptions options = {});
    ~Image();

    Image(const Image&) = delete;
    Image& operator=(const Image&) = delete;

    Image(Image&& other) noexcept;
    Image& operator=(Image&& other) noexcept;

    bool isValid() const;

    const unsigned char* data() const;
    int width() const;
    int height() const;
    int channels() const;

private:
    void destroy();

private:
    unsigned char* pixels_ = nullptr;
    int width_ = 0;
    int height_ = 0;
    int channels_ = 0;
};