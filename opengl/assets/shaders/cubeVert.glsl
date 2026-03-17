#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord; // <-- IMPORTANT: UV в location 2

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

out vec2 vUV;

void main()
{
    vUV = aTexCoord;
    gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);
}
