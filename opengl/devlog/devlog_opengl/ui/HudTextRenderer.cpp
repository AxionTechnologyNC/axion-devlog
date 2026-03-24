#include "pch.h"
#include "HudTextRenderer.h"

#define STB_EASY_FONT_IMPLEMENTATION
#include "stb_easy_font/stb_easy_font.h"

#include <Windows.h>
#include <GL/gl.h>

#pragma comment(lib, "opengl32.lib")

void HudTextRenderer::render(int screenWidth, int screenHeight)
{
    if (text_.empty())
        return;

    char buffer[9999] = {};

    const int numQuads = stb_easy_font_print(
        pos_.x,
        pos_.y,
        const_cast<char*>(text_.c_str()),
        nullptr,
        buffer,
        sizeof(buffer)
    );

    if (numQuads <= 0)
        return;

    const GLboolean depthTestEnabled = glIsEnabled(GL_DEPTH_TEST);
    const GLboolean cullFaceEnabled = glIsEnabled(GL_CULL_FACE);
    const GLboolean blendEnabled = glIsEnabled(GL_BLEND);
    const GLboolean texture2DEnabled = glIsEnabled(GL_TEXTURE_2D);

    GLint previousMatrixMode = GL_MODELVIEW;
    glGetIntegerv(GL_MATRIX_MODE, &previousMatrixMode);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0,
        static_cast<double>(screenWidth),
        static_cast<double>(screenHeight),
        0.0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor4f(color_.r, color_.g, color_.b, color_.a);

    const float* vertices = reinterpret_cast<const float*>(buffer);

    glBegin(GL_QUADS);
    for (int i = 0; i < numQuads * 4; ++i) {
        const float x = vertices[i * 4 + 0];
        const float y = vertices[i * 4 + 1];

        const float scaledX = pos_.x + (x - pos_.x) * scale_;
        const float scaledY = pos_.y + (y - pos_.y) * scale_;

        glVertex2f(scaledX, scaledY);
    }
    glEnd();

    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(previousMatrixMode);

    if (depthTestEnabled) glEnable(GL_DEPTH_TEST);
    else glDisable(GL_DEPTH_TEST);

    if (cullFaceEnabled) glEnable(GL_CULL_FACE);
    else glDisable(GL_CULL_FACE);

    if (blendEnabled) glEnable(GL_BLEND);
    else glDisable(GL_BLEND);

    if (texture2DEnabled) glEnable(GL_TEXTURE_2D);
    else glDisable(GL_TEXTURE_2D);
}