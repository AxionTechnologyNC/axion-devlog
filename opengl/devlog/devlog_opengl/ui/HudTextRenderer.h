#pragma once

#include <string>
#include <glm/glm.hpp>

class HudTextRenderer
{
public:
    void setText(const std::string& text) { text_ = text; }
    void setPosition(float x, float y) { pos_ = { x, y }; }
    void setColor(const glm::vec4& color) { color_ = color; }
    void setScale(float scale) { scale_ = scale; }

    void render(int screenWidth, int screenHeight);

private:
    std::string text_ = "Current pressed key: None";
    glm::vec2 pos_{ 20.0f, 20.0f };
    glm::vec4 color_{ 1.0f, 1.0f, 1.0f, 1.0f };
    float scale_ = 2.5f;
};