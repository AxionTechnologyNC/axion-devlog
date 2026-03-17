#version 330 core

in vec2 vUV;
out vec4 FragColor;

uniform sampler2D uTex; // logo/tex

void main()
{
    vec4 texColor = texture(uTex, vUV);

    // If PNG has alpha and we need to discard fully transparent pixels:
    // if (texColor.a < 0.01) discard;

    FragColor = texColor;
}
